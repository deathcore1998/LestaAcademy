#include "BaseCheckpoint.h"

ABaseCheckpoint::ABaseCheckpoint()
{
 	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	RootComponent = root;

	checkPointCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("checkPointCollision"));
	checkPointCollision->SetupAttachment(RootComponent);
	// Отступ от root
	FVector newPositionCollision = GetActorLocation();
	float offset = 150.f;
	newPositionCollision.Z += offset;
	checkPointCollision->SetRelativeLocation(newPositionCollision);
	// Настройки финишной линии
	checkPointLine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("checkPointLine"));
	checkPointLine->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> finishLineAsset(TEXT("StaticMesh'/Game/Meshes/Shape_Plane.Shape_Plane'"));
	checkPointLine->SetStaticMesh(finishLineAsset.Object);
	// Добавление отступа чтобы линия была над платформой
	FVector newLocationFinishLine = GetActorLocation();
	newLocationFinishLine.Z += 1;
	checkPointLine->SetRelativeLocation(newLocationFinishLine);
	// Текст финиша
	checkPointText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("finishText"));
	checkPointText->SetTextRenderColor(FColor::Red);
	checkPointText->SetupAttachment(RootComponent);
	checkPointText->SetRelativeRotation(FRotator(90.0f, 0.0f, 90.0f));
	float textSize = 100.f;
	checkPointText->SetWorldSize(textSize);
	checkPointText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	// Добавление отступа чтобы надпись была над платформой
	FVector newLocationfinishText = GetActorLocation();
	newLocationfinishText.Z += 1;
	checkPointText->SetRelativeLocation(newLocationfinishText);
}

void ABaseCheckpoint::BeginPlay()
{
	Super::BeginPlay();
}