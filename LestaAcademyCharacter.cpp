#include "LestaAcademyCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

ALestaAcademyCharacter::ALestaAcademyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// Настройка персонажа
	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.2f;
	// Создание камеры
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 500.0f;
	cameraBoom->bUsePawnControlRotation = true;
	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("followCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	followCamera->bUsePawnControlRotation = false;
	// Поиск виджета здоровья
	ConstructorHelpers::FClassFinder<UUserWidget> menuClassFinder(TEXT("/Game/Widgets/hpWidget"));
	if (menuClassFinder.Succeeded()) {
		hpWidget = menuClassFinder.Class;
	}

	maxhpCharacter = 100;
	hpCharacter = maxhpCharacter;
}

void ALestaAcademyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALestaAcademyCharacter::moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALestaAcademyCharacter::moveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ALestaAcademyCharacter::updateProgressBar()
{
	if (hpProgressBar)
	{
		// Расчет здоровья для полоски ProgressBar
		hpProgressBar->SetPercent((float)hpCharacter / maxhpCharacter);
	}	
}

void ALestaAcademyCharacter::takeDamageTrap(int damage)
{
	hpCharacter -= damage;
	updateProgressBar();
	if (hpCharacter <= 0)
	{
		gameManager->showGameLoseWidget();
		hidingWidgetHP();
	}
}

void ALestaAcademyCharacter::hidingWidgetHP()
{
	//Скрытие виджета здоровья
	hpWidgetCharacter->SetVisibility(ESlateVisibility::Collapsed);
}

void ALestaAcademyCharacter::BeginPlay()
{
	Super::BeginPlay();
	gameManager = GetWorld()->SpawnActor<AGameManager>(AGameManager::StaticClass(), FVector(0.f), FRotator(0.f));
	
	if (hpWidget)
	{
		hpWidgetCharacter = CreateWidget<UUserWidget>(GetWorld(), hpWidget);
		if (hpWidgetCharacter) {
			hpWidgetCharacter->AddToViewport();
			hpProgressBar = Cast<UProgressBar>(hpWidgetCharacter->GetWidgetFromName(TEXT("hpCharacter")));
			if (hpProgressBar)
			{
				// Изначальная установка в 1 = 100% здоровья
				hpProgressBar->SetPercent(1);
			}	
		}
	}
}

void ALestaAcademyCharacter::moveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}

void ALestaAcademyCharacter::moveRight(float value)
{
	if ( (Controller != NULL) && (value != 0.0f) )
	{		
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);		
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(direction, value);
	}
}