// Shoot Them Up Game, All Rights Reserved


#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"

ASTUPlayerController::ASTUPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}


void ASTUPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnNewPawn.Broadcast(InPawn);
}

void ASTUPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("PauseGame", IE_Pressed, this, &ASTUPlayerController::OnPauseGame);
		InputComponent->BindAction("Mute", IE_Pressed, this, &ASTUPlayerController::OnMuteSound);
	}
}

void ASTUPlayerController::OnPauseGame()
{
	if (const auto World = GetWorld())
	{
		World->GetAuthGameMode()->SetPause(this);
	}
}

void ASTUPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (const auto World = GetWorld())
	{
		if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
		}
	}
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
	if (State == ESTUMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void ASTUPlayerController::OnMuteSound()
{
	if (const auto World = GetWorld())
	{
		if (const auto GameInstance = World->GetGameInstance<USTUGameInstance>())
		{
			GameInstance->ToggleVolume();
		}
	}
}