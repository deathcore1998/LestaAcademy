#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Killzone.generated.h"

UCLASS()
class LESTAACADEMY_API AKillzone : public AActor
{
	GENERATED_BODY()
	
public:	
	AKillzone();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);

	int killzoneDamage;
	UBoxComponent* killCollision;
};
