#include "TrapRotatingStick.h"

ATrapRotatingStick::ATrapRotatingStick()
{
	PrimaryActorTick.bCanEverTick = true;

	trapStickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("trapStickMesh"));
	trapStickMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> trapStickMeshAsset(TEXT("/Game/Meshes/Cylinder.Cylinder"));
	trapStickMesh->SetStaticMesh(trapStickMeshAsset.Object);
	// Изменение размера палки
	FVector newScaleStick(0.25f, 0.25f, 3.f);
	trapStickMesh->SetRelativeScale3D(newScaleStick);
	// Поворот палки
	FRotator newRotationStick = trapStickMesh->GetComponentRotation();
	newRotationStick += FRotator(90.f, 0.f, 0.f);
	trapStickMesh->SetRelativeRotation(newRotationStick);
	// Настройка коллизии отталкивания
	FVector newScaleCollision(5.0f, 0.45f, 0.45f);
	impactCollision->SetRelativeScale3D(newScaleCollision);

	launchForce = 300.f;
	speedRotation = 130.f;
	additionallyRecliningZ = 20;
}