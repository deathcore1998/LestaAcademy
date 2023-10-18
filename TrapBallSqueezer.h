#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h" 
#include "TrapBallSqueezer.generated.h"

UCLASS()
class LESTAACADEMY_API ATrapBallSqueezer : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapBallSqueezer();
	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);

	void decreaseTrapSize(float decreaseAmount);
	void increaseTrapSize(float increaseAmount);
	void resetTrapRecharge(float deltaTime);
	void updatingTrapSize(float deltaTime);

private:
	UStaticMeshComponent* platformTrapMesh;
	UStaticMeshComponent* trapMesh;
	USphereComponent* collisionSphere;

	float additionalCollisionRadius;
	float minRadius;
	float maxRadius;
	float currentRadius;
	bool bIncreaseRadius;
	float radiusSpeedhRate;

	float rechargeTime;
	float currentRechargeTime;
	float pauseTimeTrap;
	bool bTrapWorking;
	float launchForce;
};
