#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCheckpoint.generated.h"

UCLASS()
class LESTAACADEMY_API ABaseCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseCheckpoint();

protected:
	virtual void BeginPlay() override;

	USceneComponent* root;
	UBoxComponent* checkPointCollision;
	UTextRenderComponent* checkPointText;
	UStaticMeshComponent* checkPointLine;
};
