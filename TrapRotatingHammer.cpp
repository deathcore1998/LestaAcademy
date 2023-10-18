#include "TrapRotatingHammer.h"
#include "LestaAcademyCharacter.h"

ATrapRotatingHammer::ATrapRotatingHammer()
{
	PrimaryActorTick.bCanEverTick = true;

	hammerMountingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("hammerMountingMesh"));
	hammerMountingMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> hammerMountingMeshAsset(TEXT("/Game/Meshes/Cylinder.Cylinder"));
	hammerMountingMesh->SetStaticMesh(hammerMountingMeshAsset.Object);
	// ��������� ����� ��������� 
	FVector newPositionMounting = hammerMountingMesh->GetComponentLocation();
	float additionalHeight = 25.f;
	float additionalShiftX = 50.f;
	newPositionMounting.Z += additionalHeight;
	newPositionMounting.X += additionalShiftX;
	hammerMountingMesh->SetRelativeLocation(newPositionMounting);
	// ��������� ������� ���������
	FVector newScaleStick(0.25f, 0.25f, 1.f);
	hammerMountingMesh->SetRelativeScale3D(newScaleStick);
	// ������� ���������
	FRotator newRotation = hammerMountingMesh->GetComponentRotation();
	newRotation += FRotator(90.f, 0.f, 0.f);
	hammerMountingMesh->SetRelativeRotation(newRotation);
	// �������� � ��������� ��������
	firingPinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("firingPinMesh"));
	firingPinMesh->SetupAttachment(pointRotation);
	ConstructorHelpers::FObjectFinder<UStaticMesh> firingPinMeshAsset(TEXT("/Game/Meshes/platform.platform"));
	firingPinMesh->SetStaticMesh(firingPinMeshAsset.Object);
	// ����� ������� ��������
	FVector newPositionfiringPin = hammerMountingMesh->GetComponentLocation();
	newPositionfiringPin.Z += additionalHeight;
	newPositionfiringPin.X += additionalShiftX * 2;
	firingPinMesh->SetRelativeLocation(newPositionfiringPin);
	// ��������� ������� ��������
	FVector newScaleHammerMountin(0.6f, 1.0f, 0.8f);
	firingPinMesh->SetRelativeScale3D(newScaleHammerMountin);
	// ��������� �������� ������������
	impactCollision->SetupAttachment(firingPinMesh);
	FVector newScaleCollision(60.0f, 60.0f, 60.0f);
	impactCollision->SetBoxExtent(newScaleCollision);

	launchForce = 350.f;
	speedRotation = 280.f;
	additionallyRecliningZ = 100;
}
