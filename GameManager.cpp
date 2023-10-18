#include "GameManager.h"
#include "Kismet\GameplayStatics.h"
#include "LestaAcademyCharacter.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = true;
	// Поиск всех виджетов
	ConstructorHelpers::FClassFinder<UUserWidget> gameWinFinder(TEXT("/Game/Widgets/gameWin"));
	if (gameWinFinder.Succeeded()) {
		gameWin = gameWinFinder.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> gameLoseFinder(TEXT("/Game/Widgets/gameLose"));
	if (gameLoseFinder.Succeeded()) {
		gameLose = gameLoseFinder.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> exitGameFinder(TEXT("/Game/Widgets/exitGame"));
	if (exitGameFinder.Succeeded()) {
		exitGame = exitGameFinder.Class;
	}

	gameTime = 0.0f;
	bIsTimerRunning = false;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();	

	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (exitGame)
	{
		exitGameWidget = CreateWidget<UUserWidget>(GetWorld(), exitGame);
		if (exitGameWidget) {
			exitGameWidget->AddToViewport();
		}
	}
}

void AGameManager::startGameTimer()
{
	bIsTimerRunning = true;
}

void AGameManager::stopGameTimer()
{
	bIsTimerRunning = false;
	playerController->bShowMouseCursor = true;
	playerController->bEnableClickEvents = true;
}

void AGameManager::updateGameTimer(float deltaTime)
{
	if (bIsTimerRunning)
	{
		gameTime += deltaTime;
	}
}

void AGameManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	updateGameTimer(deltaTime);
}

void AGameManager::showGameLoseWidget()
{
	stopGameTimer();
	playerController->SetPause(true);
	if (gameLose)
	{
		gameLoseWidget = CreateWidget<UUserWidget>(GetWorld(), gameLose);
		if (gameLoseWidget) {
			gameLoseWidget->AddToViewport();
		}
	}
}

void AGameManager::showGameWinWidget()
{
	ALestaAcademyCharacter* playerRef = Cast<ALestaAcademyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	playerRef->hidingWidgetHP();

	stopGameTimer();
	playerController->SetPause(true);
	if (gameWin)
	{
		gameWinWidget = CreateWidget<UUserWidget>(GetWorld(), gameWin);
		if (gameWinWidget) {
			gameWinWidget->AddToViewport();
			passageTime = Cast<UTextBlock>(gameWinWidget->GetWidgetFromName(TEXT("passageTime")));
			if (passageTime)
			{
				passageTime->SetText(FText::AsNumber(gameTime));
			}
		}
	}
}