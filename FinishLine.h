#pragma once

#include "CoreMinimal.h"
#include "BaseCheckpoint.h"
#include "FinishLine.generated.h"

UCLASS()
class LESTAACADEMY_API AFinishLine : public ABaseCheckpoint
{
	GENERATED_BODY()
	
public:
	AFinishLine();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		virtual void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);
};
