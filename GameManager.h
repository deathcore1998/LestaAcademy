#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/SWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameManager.generated.h"

UCLASS()
class LESTAACADEMY_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AGameManager();
	virtual void Tick(float deltaTime) override;

	void startGameTimer();
	void showGameWinWidget();
	void showGameLoseWidget();

protected:
	virtual void BeginPlay() override;

private:
	void stopGameTimer();
	void updateGameTimer(float deltaTime);

private:
	TSubclassOf<UUserWidget> gameWin;
	UUserWidget* gameWinWidget;
	UTextBlock* passageTime;
	TSubclassOf<UUserWidget> gameLose;
	UUserWidget* gameLoseWidget;
	TSubclassOf<UUserWidget> exitGame;
	UUserWidget* exitGameWidget;

	APlayerController* playerController;
	bool bIsTimerRunning;
	float gameTime;
};
