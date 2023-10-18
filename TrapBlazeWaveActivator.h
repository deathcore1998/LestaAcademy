#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LestaAcademyCharacter.h"
#include "TrapBlazeWaveActivator.generated.h"

// Хранение состояний ловушки
UENUM(BlueprintType)
enum class ETrapState : uint8
{
	INACTIVE,
	ACTIVE,
	HIT,
	RELOADING
};

UCLASS()
class LESTAACADEMY_API ATrapBlazeWaveActivator : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrapBlazeWaveActivator();
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

	void activateTrap();
	void hitTrap();
	void reloadingTrap();
	void resetDelay(const float& deltaTime);

private:
	UStaticMeshComponent* platformTrapMesh;
	UMaterial* orangeMaterial;
	UMaterial* redMaterial;
	UMaterial* ordinaryMaterial;

	UBoxComponent* triggerCollision;
	ALestaAcademyCharacter* playerRef;
	bool bStandingOnTrap;
	ETrapState currentState;

	float delayBefoAttack;
	float delayAfterAttack;
	float delayHit;
	float currentDelayTrap;
	int damageTrap;
};
