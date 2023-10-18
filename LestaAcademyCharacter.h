#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "LestaAcademyCharacter.generated.h"

UCLASS(config=Game)
class ALestaAcademyCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* followCamera;

public:
	ALestaAcademyCharacter();
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return cameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return followCamera; }
	void takeDamageTrap(int damage);
	void hidingWidgetHP();

protected:
	virtual void BeginPlay() override;

	void moveForward(float value);
	void moveRight(float value);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void updateProgressBar();	

	int hpCharacter;
	int maxhpCharacter;

	TSubclassOf<UUserWidget> hpWidget;
	UUserWidget* hpWidgetCharacter;
	UProgressBar* hpProgressBar;
	AGameManager* gameManager;
};

