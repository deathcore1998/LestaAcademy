#include "TrapWind.h"

ATrapWind::ATrapWind()
{
 	PrimaryActorTick.bCanEverTick = true;

	rootTrap = CreateDefaultSubobject<USceneComponent>(TEXT("rootTrap"));
	RootComponent = rootTrap;
	// ��������� ���������
	platformTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("platformTrapMesh"));
	platformTrapMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> platformMeshAsset(TEXT("/Game/Meshes/platform.platform"));
	platformTrapMesh->SetStaticMesh(platformMeshAsset.Object);
	// ������ ��������� 3�3
	FVector newScalePlatform(3.f, 3.f, 1.f);
	platformTrapMesh->SetWorldScale3D(newScalePlatform);
	// �������� ����� ��������
	pointRotation = CreateDefaultSubobject<USceneComponent>(TEXT("pointRotation"));
	pointRotation->SetupAttachment(RootComponent);
	// ��������� ������������ �������
	triggerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("triggerCollision"));
	triggerCollision->SetupAttachment(RootComponent);
	sizeHalfSideCollision = 150.f;
	float heightCollision = 100.f;
	triggerCollision->SetBoxExtent(FVector(sizeHalfSideCollision, sizeHalfSideCollision, heightCollision));
	// ��������� ������ ��������, ������ ��� ���� ��� ����������
	FVector offset = GetActorLocation();
	offset.Z += sizeHalfSideCollision;
	triggerCollision->SetRelativeLocation(offset);
	// ��������� �����������
	fanMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("fanMesh"));
	fanMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> fanMeshAsset(TEXT("/Game/Meshes/Colling_Fan.Colling_Fan"));
	fanMesh->SetStaticMesh(fanMeshAsset.Object);
	fanMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	// ��������� ������� ����������� ��� ���������
	FVector newScaleFan(2.f, 1.f, 2.f);
	fanMesh->SetWorldScale3D(newScaleFan);
	// ����� ����������� � ���� �������
	offset = GetActorLocation();
	offset.Z += heightCollision;
	offset.Y -= sizeHalfSideCollision;
	fanMesh->SetRelativeLocation(offset);
	// ������ ��������� ����������� �����
	rotationAngles = {0.f, -90.f, 180.f, 90.f };

	bStandingOnTrap = false;
	forceWind = 1.8f;
	delayWindChange = 2.f;
	currentDelayWindChange = 0.f;

	angleRotation = 0.f;
	bFanTurns = true;
	alreadyTurned = 0.0f;
	turningSpeedFan = 100.f;
	startRotation = 0.f;
}

void ATrapWind::BeginPlay()
{
	Super::BeginPlay();

	triggerCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapWind::onCharacterCollisionBegin);
	triggerCollision->OnComponentEndOverlap.AddDynamic(this, &ATrapWind::onCharacterCollisionEnd);
}

void ATrapWind::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		// ���� ��� ��������� �� ����� ���, �� �������� ���
		if (!playerRef)
		{
			playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		}
		bStandingOnTrap = true;
	}
}

void ATrapWind::onCharacterCollisionEnd(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		bStandingOnTrap = false;
	}
}

void ATrapWind::resetRefreshTimer(float deltaTime)
{
	if (!bFanTurns)
	{
		currentDelayWindChange -= deltaTime;
		if (currentDelayWindChange <= 0)
		{
			turnSelection();			
			startRotation = pointRotation->GetComponentRotation().Yaw;
			bFanTurns = true;
			currentDelayWindChange = delayWindChange;
		}
	}
}

void ATrapWind::updateFanRotation(float deltaTime)
{
	if (bFanTurns)
	{		
		FRotator newRotationWind = pointRotation->GetComponentRotation();
		// �������� ���� < 0 ��� ���
		// � ����������� �� ����� ���� ���������� �������� � ������ �������
		if (angleRotation < 0)
		{
			alreadyTurned -= deltaTime * turningSpeedFan;

			if (alreadyTurned > angleRotation)
			{
				newRotationWind.Yaw = startRotation + alreadyTurned;
			}
			else
			{
				bFanTurns = false;
			}
		}
		else
		{
			alreadyTurned += deltaTime * turningSpeedFan;

			if (alreadyTurned < angleRotation)
			{
				newRotationWind.Yaw = startRotation + alreadyTurned;
			}
			else
			{				
				bFanTurns = false;				
			}
		}

		if (!bFanTurns)
		{
			newRotationWind.Yaw = startRotation + angleRotation;
			alreadyTurned = 0;
		}

		pointRotation->SetRelativeRotation(newRotationWind);
	}
}

void ATrapWind::blownAwayWindCharacter()
{
	if (bStandingOnTrap && playerRef )
	{
		// ������� ����������� ���� �����
		float currentAngle = pointRotation->GetComponentRotation().Yaw + 90;
		float pointRadian = FMath::DegreesToRadians(currentAngle);
		FVector fanDirection = FVector(FMath::Cos(pointRadian), FMath::Sin(pointRadian), 0.f);
		// ���������� ���� ����� �� ���������
		FVector positionCharacte = playerRef->GetActorLocation();
		positionCharacte += fanDirection * forceWind;
		playerRef->SetActorLocation(positionCharacte);
	}
}

void ATrapWind::turnSelection()
{
	// ����� ������ ������� �����������
	int32 randomIndex = FMath::RandRange(0, 3);
	angleRotation = rotationAngles[randomIndex];
}

void ATrapWind::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	resetRefreshTimer(deltaTime);
	updateFanRotation(deltaTime);
	blownAwayWindCharacter();
}