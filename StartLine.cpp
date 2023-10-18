#include "StartLine.h"
#include "LestaAcademyCharacter.h"
#include "GameManager.h"

AStartLine::AStartLine()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FVector boxCollisionSize(500.f, 10.f, 150.f);
	checkPointCollision->SetBoxExtent(boxCollisionSize);

	UMaterial* finishMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/MyMaterial/RedMaterial.RedMaterial")).Get();
	checkPointLine->SetMaterial(0, finishMaterial);
	// Изменение размера стартового блока
	FVector newSCaleFinishLine(10.0f, 0.25f, 1.0f);
	checkPointLine->SetRelativeScale3D(newSCaleFinishLine);
	// Текст старта
	FText textStart = FText::FromString(TEXT("START"));
	checkPointText->SetText(textStart);

	bGameHasStarted = false;
}

void AStartLine::BeginPlay()
{
	ABaseCheckpoint::BeginPlay();
	checkPointCollision->OnComponentBeginOverlap.AddDynamic(this, &AStartLine::onCharacterCollisionBegin);
}

void AStartLine::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Если еще стартовая линия не пересечена
	if (!bGameHasStarted)
	{
		if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
		{
			bGameHasStarted = true;
			AGameManager* gameManager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
			if (gameManager)
			{
				gameManager->startGameTimer();
			}
		}
	}
}