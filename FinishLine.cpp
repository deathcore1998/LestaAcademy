#include "FinishLine.h"
#include "LestaAcademyCharacter.h"
#include "GameManager.h"

AFinishLine::AFinishLine()
{
	PrimaryActorTick.bCanEverTick = true;

	FVector boxCollisionSize(150.f, 10.f, 150.f);
	checkPointCollision->SetBoxExtent(boxCollisionSize);

	UMaterial* finishMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/finishMaterial.finishMaterial")).Get();
	checkPointLine->SetMaterial(0, finishMaterial);
	// Изменение размера финишного блока
	FVector newSCaleFinishLine(3.0f, 1.0f, 1.0f);
	checkPointLine->SetRelativeScale3D(newSCaleFinishLine);
	// Текст финиша
	FText textFinish = FText::FromString(TEXT("FINISH"));
	checkPointText->SetText(textFinish);
	// Добавление отступа
	FVector newLocationfinishText = GetActorLocation();
	newLocationfinishText.Z += 1;
	// Отступ чтобы надпись не лежала на финишной линии
	newLocationfinishText.Y += 50.f;
	checkPointText->SetRelativeLocation(newLocationfinishText);
}

void AFinishLine::BeginPlay()
{
	ABaseCheckpoint::BeginPlay();
	checkPointCollision->OnComponentBeginOverlap.AddDynamic(this, &AFinishLine::onCharacterCollisionBegin);
}

void AFinishLine::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		AGameManager* gameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
		if (gameManager)
		{
			gameManager->showGameWinWidget();
		}
	}
}