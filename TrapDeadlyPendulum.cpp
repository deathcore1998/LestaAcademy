#include "TrapDeadlyPendulum.h"
#include "LestaAcademyCharacter.h"

ATrapDeadlyPendulum::ATrapDeadlyPendulum()
{
 	PrimaryActorTick.bCanEverTick = true;
	// ����� �������� � ������� ����� ����������� ��������� ��������
	pointRotation = CreateDefaultSubobject<USceneComponent>(TEXT("pointRotation"));
	RootComponent = pointRotation;
	// �������� ��������� ��� ����
	fixBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("fixBallMesh"));
	fixBallMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> fixBallMeshAsset(TEXT("/Game/Meshes/Cylinder.Cylinder"));
	fixBallMesh->SetStaticMesh(fixBallMeshAsset.Object);
	// ��������� �������
	FVector newScaleFixBall(0.25f, 0.25f, 5.f);
	fixBallMesh->SetRelativeScale3D(newScaleFixBall);
	// ��������� ������� ��������� �� ����� ��������
	float additionalDistance = 250.f;
	FVector newLocationFix = GetActorLocation();
	newLocationFix.Z -= additionalDistance;
	fixBallMesh->SetRelativeLocation(newLocationFix);
	// �������� ����
	ballTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ballTrapMesh"));
	ballTrapMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> ballTrapMeshAsset(TEXT("/Game/Meshes/Sphere.Sphere"));
	ballTrapMesh->SetStaticMesh(ballTrapMeshAsset.Object);
	// ��������� �������
	FVector newScaleBall(2.f, 2.f, 2.f);
	ballTrapMesh->SetRelativeScale3D(newScaleBall);
	// ��������� ������� ���� �� ����� ��������
	FVector newLocationBall = GetActorLocation();
	newLocationBall.Z -= 2 * additionalDistance;
	ballTrapMesh->SetRelativeLocation(newLocationBall);
	// ��������� �������� ������� ����������� ������
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));
	collisionSphere->SetupAttachment(ballTrapMesh);
	float newRadiusCollision = 60.f;
	collisionSphere->InitSphereRadius(newRadiusCollision);
	// ��������� ���������� ��������� � ������ �������� ��������
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
		// ��������� ������ �� ���������
		ALestaAcademyCharacter* playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		if (playerRef)
		{
			// ������� ����������� ���� ����������
			FVector launchDirection = playerRef->GetActorLocation() - collisionSphere->GetComponentLocation();
			launchDirection.Normalize();
			// ���������� �����������
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
	// ������� ����� ��������
	FRotator newRotation = FRotator(forcePush * FMath::RadiansToDegrees(FMath::Cos(currentPeriod)), 0.0f, 0.0f);
	pointRotation->SetRelativeRotation(newRotation);
}