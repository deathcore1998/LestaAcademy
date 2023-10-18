#pragma once

#include "CoreMinimal.h"
#include "BaseCheckpoint.h"
#include "StartLine.generated.h"

UCLASS()
class LESTAACADEMY_API AStartLine : public ABaseCheckpoint
{
	GENERATED_BODY()
	
public:
	AStartLine();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);

	bool bGameHasStarted;
};
