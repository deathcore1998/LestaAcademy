#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TrapBasicRotating.generated.h"

UCLASS(ABSTRACT)
class LESTAACADEMY_API ATrapBasicRotating : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapBasicRotating();
	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);
    void rotationLogicTrap(float deltaTime);

protected:
	UStaticMeshComponent* platformTrapMesh;
	UStaticMeshComponent* rotatingPartTrapMesh;
	USceneComponent* pointRotation;
	UBoxComponent* impactCollision;
	
	UPROPERTY(EditAnywhere)
		bool bClockwiseRotation;
	float launchForce;
	float speedRotation;
	float additionallyRecliningZ;
	float maximumPushZ;
};
