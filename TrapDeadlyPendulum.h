#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h" 
#include "TrapDeadlyPendulum.generated.h"

UCLASS()
class LESTAACADEMY_API ATrapDeadlyPendulum : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapDeadlyPendulum();
	virtual void Tick(float deltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void moveBallTrap(float deltaTime);
	UFUNCTION()
		void onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
			UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult);

private:
	USceneComponent* pointRotation;
	UStaticMeshComponent* fixBallMesh;
	UStaticMeshComponent* ballTrapMesh;
	USphereComponent* collisionSphere;

	float currentPeriod;
	float startPeriod;
	float endPeriod;
	float forcePush;
	UPROPERTY(EditAnywhere)
		bool bMovementLeft;
	float launchForce;
};
