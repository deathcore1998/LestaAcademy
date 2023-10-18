#pragma once

#include "CoreMinimal.h"
#include "TrapBasicRotating.h"
#include "TrapRotatingHammer.generated.h"

UCLASS()
class LESTAACADEMY_API ATrapRotatingHammer : public ATrapBasicRotating
{
	GENERATED_BODY()
	
public:
	ATrapRotatingHammer();

private:
	UStaticMeshComponent* hammerMountingMesh;
	UStaticMeshComponent* firingPinMesh;
};
