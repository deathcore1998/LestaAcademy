#include "LestaAcademyGameMode.h"
#include "LestaAcademyCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALestaAcademyGameMode::ALestaAcademyGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
