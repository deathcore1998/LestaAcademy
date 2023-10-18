#include "TrapDeadlyPendulum.h"
#include "LestaAcademyCharacter.h"

ATrapDeadlyPendulum::ATrapDeadlyPendulum()
{
 	PrimaryActorTick.bCanEverTick = true;
	// Точка вращения к которой будут прикреплены остальные элементы
	pointRotation = CreateDefaultSubobject<USceneComponent>(TEXT("pointRotation"));
	RootComponent = pointRotation;
	// Создание крепления для шара
	fixBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("fixBallMesh"));
	fixBallMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> fixBallMeshAsset(TEXT("/Game/Meshes/Cylinder.Cylinder"));
	fixBallMesh->SetStaticMesh(fixBallMeshAsset.Object);
	// Изменение размера
	FVector newScaleFixBall(0.25f, 0.25f, 5.f);
	fixBallMesh->SetRelativeScale3D(newScaleFixBall);
	// Изменение отступа крепления от точки вращения
	float additionalDistance = 250.f;
	FVector newLocationFix = GetActorLocation();
	newLocationFix.Z -= additionalDistance;
	fixBallMesh->SetRelativeLocation(newLocationFix);
	// Создание шара
	ballTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ballTrapMesh"));
	ballTrapMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> ballTrapMeshAsset(TEXT("/Game/Meshes/Sphere.Sphere"));
	ballTrapMesh->SetStaticMesh(ballTrapMeshAsset.Object);
	// Изменение размера
	FVector newScaleBall(2.f, 2.f, 2.f);
	ballTrapMesh->SetRelativeScale3D(newScaleBall);
	// Изменение отступа шара от точки вращения
	FVector newLocationBall = GetActorLocation();
	newLocationBall.Z -= 2 * additionalDistance;
	ballTrapMesh->SetRelativeLocation(newLocationBall);
	// Настройка коллизии которая отбрасывает игрока
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));
	collisionSphere->SetupAttachment(ballTrapMesh);
	float newRadiusCollision = 60.f;
	collisionSphere->InitSphereRadius(newRadiusCollision);
	// Установка начального положения и границ движения маятника
	startPeriod = PI;
	endPeriod = 2 * PI;
	currentPeriod = 3 * PI / 2;

	forcePush = 1.4f;
	bMovementLeft = false;
	launchForce = 650;
}

void ATrapDeadlyPendulum::BeginPlay()
{
	Super::BeginPlay();
	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATrapDeadlyPendulum::onCharacterCollisionBegin);
}

void ATrapDeadlyPendulum::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		// Получение ссылки на персонажа
		ALestaAcademyCharacter* playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		if (playerRef)
		{
			// Рассчет направления куда откидывать
			FVector launchDirection = playerRef->GetActorLocation() - collisionSphere->GetComponentLocation();
			launchDirection.Normalize();
			// Применение откидывания
			playerRef->LaunchCharacter(launchDirection * launchForce, false, false);
		}
	}
}

void ATrapDeadlyPendulum::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	moveBallTrap(deltaTime);
}


void ATrapDeadlyPendulum::moveBallTrap(float deltaTime)
{
	if (currentPeriod + deltaTime > endPeriod) {
		bMovementLeft = true;
	}
	else if (currentPeriod < startPeriod) {
		bMovementLeft = false;
	}

	if (bMovementLeft) {
		currentPeriod -= deltaTime;
	}
	else {
		currentPeriod += deltaTime;
	}
	// Поворот точки вращения
	FRotator newRotation = FRotator(forcePush * FMath::RadiansToDegrees(FMath::Cos(currentPeriod)), 0.0f, 0.0f);
	pointRotation->SetRelativeRotation(newRotation);
}