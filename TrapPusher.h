#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TrapPusher.generated.h"

UCLASS()
class LESTAACADEMY_API ATrapPusher : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapPusher();
	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);
	void resetFlashingTime(float deltaTime);
	void enablingFlashing();

private:
	UStaticMeshComponent* platformTrapMesh;
	UMaterial* blueMaterial;
	UMaterial* ordinaryMaterial;
	UBoxComponent* triggerCollision;

	float magnitudePush;
	float flashingTime;
	float currentFlashingTime;
	bool bTrapHighlighted;
};
