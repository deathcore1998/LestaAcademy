#pragma once

#include "CoreMinimal.h"
#include "TrapBasicRotating.h"
#include "TrapRotatingStick.generated.h"

UCLASS()
class LESTAACADEMY_API ATrapRotatingStick : public ATrapBasicRotating
{
	GENERATED_BODY()
	
public:
	ATrapRotatingStick();

private:
	UStaticMeshComponent* trapStickMesh;
};
