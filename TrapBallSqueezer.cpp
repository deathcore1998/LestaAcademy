#include "TrapBallSqueezer.h"
#include "LestaAcademyCharacter.h"

ATrapBallSqueezer::ATrapBallSqueezer()
{
	PrimaryActorTick.bCanEverTick = true;
    // Установка платформы ловушки
    platformTrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("platformTrapMesh"));
    RootComponent = platformTrapMesh;
    ConstructorHelpers::FObjectFinder<UStaticMesh>  platformMeshAsset(TEXT("/Game/Meshes/platform.platform"));
    platformTrapMesh->SetStaticMesh(platformMeshAsset.Object);
    // Настройка коллизии которая отбрасывает игрока
    collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));
    collisionSphere->SetupAttachment(RootComponent);
    additionalCollisionRadius = 10;
    minRadius = 50.0f;
    maxRadius = 150.0f;
    currentRadius = minRadius;
    collisionSphere->InitSphereRadius(minRadius + additionalCollisionRadius);
    // Поднятие сферы над платформой
    FVector offset = GetActorLocation();
    offset.Z += minRadius * 2;
    collisionSphere->SetRelativeLocation(offset);
    // Визуальная сфера
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
    //Множитель силы отталкивания
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
        // Увеличение или уменьшение радиуса сферы
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
    // Смещение по оси Z пропорционально увеличению радиуса
    FVector newLocationSphere = collisionSphere->GetComponentLocation();
    newLocationSphere.Z += increaseAmount;
    collisionSphere->SetWorldLocation(newLocationSphere);
    // Увеличение сферы, которая отбрасывает игрока
    collisionSphere->SetSphereRadius(currentRadius + additionalCollisionRadius);
    // Увеличение визуальной сферы
    FVector newScale(currentRadius / minRadius);
    trapMesh->SetRelativeScale3D(newScale);
}

void ATrapBallSqueezer::decreaseTrapSize(float decreaseAmount)
{
    // Смещение по оси Z пропорционально уменьшению радиуса
    FVector newLocationSphere = collisionSphere->GetComponentLocation();
    newLocationSphere.Z -= decreaseAmount;
    collisionSphere->SetWorldLocation(newLocationSphere);
    // Уменьшение сферы, которая отбрасывает игрока
    collisionSphere->SetSphereRadius(currentRadius + additionalCollisionRadius);
    // Уменьшение визуальной сферы
    FVector newScale(currentRadius / minRadius);
    trapMesh->SetRelativeScale3D(newScale);
}