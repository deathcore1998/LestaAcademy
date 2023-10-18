#include "TrapPusher.h"
#include "LestaAcademyCharacter.h"

ATrapPusher::ATrapPusher()
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
	// Установка коллизии над платформой
	FVector offset = GetActorLocation();
	offset.Z += heightCollision + sizeSideCollision;
	triggerCollision->SetRelativeLocation(offset);
	//Загрузка материалов
	blueMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/BlueMaterial.BlueMaterial")).Get();
	ordinaryMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/OrdinaryMaterial.OrdinaryMaterial")).Get();

	magnitudePush = 700.f;
	flashingTime = 0.33;
	currentFlashingTime = 0.0f;
	bTrapHighlighted = false;
}

void ATrapPusher::BeginPlay()
{
	Super::BeginPlay();
	triggerCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapPusher::onCharacterCollisionBegin);
}

void ATrapPusher::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		// Получение ссылки на персонажа
		ALestaAcademyCharacter* playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		if (playerRef)
		{
			enablingFlashing();
			FVector launchDirection(0.f, 0.f, magnitudePush);
			// Применение откидывания
			playerRef->LaunchCharacter(launchDirection, false, false);
		}
	}
}

void ATrapPusher::resetFlashingTime(float deltaTime)
{
	if (bTrapHighlighted)
	{
		currentFlashingTime -= deltaTime;
		if (currentFlashingTime <= 0)
		{
			platformTrapMesh->SetMaterial(0, ordinaryMaterial);
			bTrapHighlighted = false;
		}
	}
}

void ATrapPusher::enablingFlashing()
{
	bTrapHighlighted = true;
	platformTrapMesh->SetMaterial(0, blueMaterial);
	currentFlashingTime = flashingTime;
}

void ATrapPusher::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	resetFlashingTime(deltaTime);
}

