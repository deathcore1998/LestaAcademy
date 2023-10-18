#include "TrapBlazeWaveActivator.h"
#include "Kismet\GameplayStatics.h"

ATrapBlazeWaveActivator::ATrapBlazeWaveActivator()
{
	PrimaryActorTick.bCanEverTick = true;

	platformTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("platformTrapMesh"));
	RootComponent = platformTrapMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> platforMeshAsset(TEXT("/Game/Meshes/platform.platform"));
	platformTrapMesh->SetStaticMesh(platforMeshAsset.Object);
	// Настройка коллизии
	triggerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("triggerCollision"));
	triggerCollision->SetupAttachment(RootComponent);
	float sizeSideCollision = 48.f;
	float heightCollision = 10.f;
	triggerCollision->SetBoxExtent(FVector(sizeSideCollision, sizeSideCollision, heightCollision));
	// Установка ловушки над платформой
	FVector offset = GetActorLocation();
	offset.Z += heightCollision + sizeSideCollision;
	triggerCollision->SetRelativeLocation(offset);
	//Загрузка материалов для различных состояний
    orangeMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/OrangeMaterial.OrangeMaterial")).Get();
	redMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/RedMaterial.RedMaterial")).Get();
	ordinaryMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/OrdinaryMaterial.OrdinaryMaterial")).Get();

	bStandingOnTrap = false;
	currentState = ETrapState::INACTIVE;
	delayBefoAttack = 1.f;
	delayHit = 0.1f;
	delayAfterAttack = 5.f;
	currentDelayTrap = 0.f;
	damageTrap = 10;
}

void ATrapBlazeWaveActivator::BeginPlay()
{
	Super::BeginPlay();
	triggerCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapBlazeWaveActivator::onCharacterCollisionBegin);
	triggerCollision->OnComponentEndOverlap.AddDynamic(this, &ATrapBlazeWaveActivator::onCharacterCollisionEnd);
}

void ATrapBlazeWaveActivator::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		if (!playerRef)
		{
			playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		}
		bStandingOnTrap = true;
	}
}

void ATrapBlazeWaveActivator::onCharacterCollisionEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{		
		bStandingOnTrap = false;
	}
}

void ATrapBlazeWaveActivator::activateTrap()
{
	currentState = ETrapState::ACTIVE;
	currentDelayTrap = delayBefoAttack;
	platformTrapMesh->SetMaterial(0, orangeMaterial);
}

void ATrapBlazeWaveActivator::hitTrap()
{
	currentState = ETrapState::HIT;
	currentDelayTrap = delayHit;
	platformTrapMesh->SetMaterial(0, redMaterial);

	if (playerRef && bStandingOnTrap)
	{
		playerRef->takeDamageTrap(damageTrap);
	}
}

void ATrapBlazeWaveActivator::reloadingTrap()
{
	currentState = ETrapState::RELOADING;
	currentDelayTrap = delayAfterAttack;
	platformTrapMesh->SetMaterial(0, ordinaryMaterial);
}

void ATrapBlazeWaveActivator::resetDelay(const float& deltaTime)
{
	if (currentState != ETrapState::INACTIVE)
	{
		currentDelayTrap -= deltaTime;
		if (currentDelayTrap <= 0)
		{
			switch (currentState)
			{
			case ETrapState::ACTIVE:
				hitTrap();
				break;

			case ETrapState::HIT:
				reloadingTrap();
				break;

			case ETrapState::RELOADING:
				currentState = ETrapState::INACTIVE;
				break;
			}
		}
	}
	else if(currentState == ETrapState::INACTIVE && bStandingOnTrap)
	{
		activateTrap();
	}
}

void ATrapBlazeWaveActivator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	resetDelay(deltaTime);
}