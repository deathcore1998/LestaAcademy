#include "Killzone.h"
#include "LestaAcademyCharacter.h"

AKillzone::AKillzone()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	killCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("killCollision"));
	RootComponent = killCollision;
	FVector startSizeCollision(100.f, 100.f, 10.f);
	killCollision->SetBoxExtent(startSizeCollision);

	killzoneDamage = 1000;
}

void AKillzone::BeginPlay()
{
	Super::BeginPlay();
	killCollision->OnComponentBeginOverlap.AddDynamic(this, &AKillzone::onCharacterCollisionBegin);
}

void AKillzone::onCharacterCollisionBegin(UPrimitiveComponent* overlappendComp, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodeIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor->IsA(ALestaAcademyCharacter::StaticClass()))
	{
		ALestaAcademyCharacter* playerRef = Cast<ALestaAcademyCharacter>(otherActor);
		if (playerRef)
		{
			playerRef->takeDamageTrap(killzoneDamage);
		}
	}
}