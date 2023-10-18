#include "TrapBallSqueezer.h"
#include "LestaAcademyCharacter.h"

ATrapBallSqueezer::ATrapBallSqueezer()
{
	PrimaryActorTick.bCanEverTick = true;
    // ��������� ��������� �������
    platformTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("platformTrapMesh"));
    RootComponent = platformTrapMesh;
    ConstructorHelpers::FObjectFinder<UStaticMesh>  platformMeshAsset(TEXT("/Game/Meshes/platform.platform"));
    platformTrapMesh->SetStaticMesh(platformMeshAsset.Object);
    // ��������� �������� ������� ����������� ������
    collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));
    collisionSphere->SetupAttachment(RootComponent);
    additionalCollisionRadius = 10;
    minRadius = 50.0f;
    maxRadius = 150.0f;
    currentRadius = minRadius;
    collisionSphere->InitSphereRadius(minRadius + additionalCollisionRadius);
    // �������� ����� ��� ����������
    FVector offset = GetActorLocation();
    offset.Z += minRadius * 2;
    collisionSphere->SetRelativeLocation(offset);
    // ���������� �����
	trapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("trapMesh"));
    trapMesh->SetupAttachment(collisionSphere);
	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMeshAsset(TEXT("/Game/Meshes/Sphere.Sphere"));
	trapMesh->SetStaticMesh(sphereMeshAsset.Object);

    bIncreaseRadius = true;
    rechargeTime = 5.f;
    currentRechargeTime = 0.f;
    pauseTimeTrap = 1.0f;
    bTrapWorking = true;;
    radiusSpeedhRate = 400.f;
    //��������� ���� ������������
    launchForce = 450.0f;
}

void ATrapBallSqueezer::BeginPlay()
{
	Super::BeginPlay();
    collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATrapBallSqueezer::onCharacterCollisionBegin);
}

void ATrapBallSqueezer::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
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

void ATrapBallSqueezer::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
    updatingTrapSize(deltaTime);
    resetTrapRecharge(deltaTime);
}

void ATrapBallSqueezer::updatingTrapSize(float deltaTime)
{
    if (bTrapWorking)
    {
        float changingRadius = deltaTime * radiusSpeedhRate;
        // ���������� ��� ���������� ������� �����
        if (bIncreaseRadius)
        {
            currentRadius += changingRadius;
            increaseTrapSize(changingRadius);
            if (currentRadius >= maxRadius)
            {
                bIncreaseRadius = false;
                currentRechargeTime = pauseTimeTrap;
                bTrapWorking = false;
            }
        }
        else
        {
            currentRadius -= changingRadius;
            decreaseTrapSize(changingRadius);
            if (currentRadius <= minRadius)
            {
                bIncreaseRadius = true;
                currentRechargeTime = rechargeTime;
                bTrapWorking = false;
            }
        }
    } 
}

void ATrapBallSqueezer::resetTrapRecharge(float deltaTime)
{
    if (!bTrapWorking)
    {
        currentRechargeTime -= deltaTime;
        if (currentRechargeTime <= 0)
        {
            bTrapWorking = true;
        }
    }
}

void ATrapBallSqueezer::increaseTrapSize(float increaseAmount)
{
    // �������� �� ��� Z ��������������� ���������� �������
    FVector newLocationSphere = collisionSphere->GetComponentLocation();
    newLocationSphere.Z += increaseAmount;
    collisionSphere->SetWorldLocation(newLocationSphere);
    // ���������� �����, ������� ����������� ������
    collisionSphere->SetSphereRadius(currentRadius + additionalCollisionRadius);
    // ���������� ���������� �����
    FVector newScale(currentRadius / minRadius);
    trapMesh->SetRelativeScale3D(newScale);
}

void ATrapBallSqueezer::decreaseTrapSize(float decreaseAmount)
{
    // �������� �� ��� Z ��������������� ���������� �������
    FVector newLocationSphere = collisionSphere->GetComponentLocation();
    newLocationSphere.Z -= decreaseAmount;
    collisionSphere->SetWorldLocation(newLocationSphere);
    // ���������� �����, ������� ����������� ������
    collisionSphere->SetSphereRadius(currentRadius + additionalCollisionRadius);
    // ���������� ���������� �����
    FVector newScale(currentRadius / minRadius);
    trapMesh->SetRelativeScale3D(newScale);
}