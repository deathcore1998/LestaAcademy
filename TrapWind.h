#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LestaAcademyCharacter.h"
#include "TrapWind.generated.h"

UCLASS()
class LESTAACADEMY_API ATrapWind : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapWind();
	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
		void onCharacterCollisionEnd(class UPrimitiveComponent* overlappedComp,
			class AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	void resetRefreshTimer(float deltaTime);
	void updateFanRotation(float deltaTime);
	void blownAwayWindCharacter();
	void turnSelection();

private:
	UStaticMeshComponent* platformTrapMesh;
	UStaticMeshComponent* fanMesh;	
	USceneComponent* rootTrap;

	UBoxComponent* triggerCollision;
	ALestaAcademyCharacter* playerRef;
	float sizeHalfSideCollision;
	bool bStandingOnTrap;

	USceneComponent* pointRotation;
	TArray<float> rotationAngles;
	float forceWind;
	float turningSpeedFan;
	bool bFanTurns;
	float alreadyTurned;
	float startRotation;
	float angleRotation;

	float delayWindChange;
	float currentDelayWindChange;
};
