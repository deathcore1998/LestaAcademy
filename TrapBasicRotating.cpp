#include "TrapBasicRotating.h"
#include "LestaAcademyCharacter.h"

ATrapBasicRotating::ATrapBasicRotating()
{
	PrimaryActorTick.bCanEverTick = true;
	// Настройки платформы
	platformTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("platformTrapMesh"));
	RootComponent = platformTrapMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> platforMeshAsset(TEXT("/Game/Meshes/platform.platform"));
	platformTrapMesh->SetStaticMesh(platforMeshAsset.Object);
	// Настройка точки вращения
	pointRotation = CreateDefaultSubobject<USceneComponent>(TEXT("pointRotation"));
	pointRotation->SetupAttachment(RootComponent);

	rotatingPartTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rotatingPartTrapMesh"));
	rotatingPartTrapMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> rotatingPartTrapMeshAsset(TEXT("/Game/Meshes/Cylinder.Cylinder"));
	rotatingPartTrapMesh->SetStaticMesh(rotatingPartTrapMeshAsset.Object);
	// Отступ, чтобы вращающаяся часть была над платформой
	FVector offsePointRotation = GetActorLocation();
	float offset = 100.f;
	offsePointRotation.Z += offset;
	pointRotation->SetRelativeLocation(offsePointRotation);
	// Новый масштаб для rotatingPartTrapMesh
	FVector newScaleRotatingPart(0.5f, 0.5f, 1.0f);
	rotatingPartTrapMesh->SetWorldScale3D(newScaleRotatingPart);

	impactCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("impactCollision"));	
	impactCollision->SetupAttachment(pointRotation);

	bClockwiseRotation = true;
	additionallyRecliningZ = 0;
	maximumPushZ = 400.f;
}

void ATrapBasicRotating::BeginPlay()
{
	Super::BeginPlay();
	impactCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapBasicRotating::onCharacterCollisionBegin);
	// Если вращение против часовой, то скорость меняется на отрицательную
	if (!bClockwiseRotation)
	{
		speedRotation *= -1.0f;
	}
}

void ATrapBasicRotating::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		// Получение ссылки на персонажа
		ALestaAcademyCharacter* playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		if (playerRef)
		{
			// Рассчет направления куда откидывать
			FVector launchDirection = playerRef->GetActorLocation() - impactCollision->GetComponentLocation();
			// При необходимости добавление откидывание вверх, по умолчанию оно 0
			launchDirection.Z += additionallyRecliningZ;
			// Чтобы откидывание не было слишком сильным

			if (launchDirection.Z > maximumPushZ)
			{
				launchDirection.Z = maximumPushZ;
			}
			launchDirection.Normalize();
			// Применение откидывания
			playerRef->LaunchCharacter(launchDirection * launchForce, false, false);
		}
	}
}

void ATrapBasicRotating::rotationLogicTrap(float deltaTime)
{
	FRotator newRotation = pointRotation->GetComponentRotation();
	newRotation += FRotator(0.f, speedRotation * deltaTime, 0.f);
	pointRotation->SetRelativeRotation(newRotation);
}

void ATrapBasicRotating::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	rotationLogicTrap(deltaTime);
}