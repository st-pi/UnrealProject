// Shoot Them Up Game, All Rights Reserved


#include "STUGameModeBase.h"

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "Player/STUPlayerState.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"
#include "Components/STUWeaponComponent.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY_STATIC(BaseGameModLog, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHud::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeams();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ESTUMatchState::InProgress);
}

void ASTUGameModeBase::SpawnBots()
{
	if (const auto World = GetWorld())
	{
		for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			const auto AIController = World->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
			RestartPlayer(AIController);
		}
	}
}


UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	bool PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		SetMatchState(ESTUMatchState::Pause);
		StopAllFire();
	}
	return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
	bool bResult = Super::ClearPause();
	if (bResult)
	{
		SetMatchState(ESTUMatchState::InProgress);
	}
	return bResult;
}

void ASTUGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::UpdateGameTimer, 1.0f, true);
}

void ASTUGameModeBase::UpdateGameTimer() 
{
	//const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ASTUGameModeBase::ResetPlayers()
{
	if (auto World = GetWorld())
	{
		for (auto It = World->GetControllerIterator(); It; ++It)
		{
			ResetOnePlayer(It->Get());
		}
	}
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeams()
{
	if (auto World = GetWorld())
	{
		int32 TeamID = 1;
		for (auto It = World->GetControllerIterator(); It; ++It)
		{
			const auto Controller = It->Get();
			if (!Controller || !Controller->GetPawn())
			{
				continue;
			}
			const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
			if (!PlayerState)
			{
				continue;
			}
			PlayerState->SetTeamID(TeamID);
			PlayerState->SetTeamColor(GetColorByTeamID(TeamID));
			PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
			SetPlayerColor(Controller);

			UE_LOG(BaseGameModLog, Display, TEXT("CreateTeams:: %s has teamid %d, team color %s"), *Controller->GetName(), TeamID, *PlayerState->GetTeamColor().ToString());

			TeamID = TeamID == 1 ? 2 : 1;
		}
	}
}

FLinearColor ASTUGameModeBase::GetColorByTeamID(int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}

	return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) 
	{
		return;
	}
	const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if (!Character)
	{
		return;
	}
	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if (!PlayerState)
	{
		return;
	}
	UE_LOG(BaseGameModLog, Display, TEXT("SetPlayerColor:: %s has teamid %d, team color %s"), *Controller->GetName(), PlayerState->GetTeamID(), *PlayerState->GetTeamColor().ToString());
	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(AController* Killer, AController* Victim)
{
	const auto KillerPlayerState = Killer ? Cast<ASTUPlayerState>(Killer->PlayerState) : nullptr;
	const auto VictimPlayerState = Victim ? Cast<ASTUPlayerState>(Victim->PlayerState) : nullptr;
	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}
	StartRespawn(Victim);
}

FGameData ASTUGameModeBase::GetGamaData() const
{
	return GameData;
}
int32 ASTUGameModeBase::GetCurrentRoundNum() const
{
	return CurrentRound;
}
int32 ASTUGameModeBase::GetTimeleft() const
{
	return RoundCountDown;
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
	if (RoundCountDown > GameData.MinTimeleftForRespawn + GameData.RespawnTime)
	{
		if (const auto RespawnComponent = Controller->FindComponentByClass<USTURespawnComponent>())
		{
			RespawnComponent->Respawn(GameData.RespawnTime);
		}
	}
}

void ASTUGameModeBase::GameOver()
{
	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
	if (MatchState != State)
	{
		MatchState = State;
		OnMatchStateChanged.Broadcast(MatchState);
	}
}

void ASTUGameModeBase::StopAllFire()
{
	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (const auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>())
		{
			WeaponComponent->StopFire();
			WeaponComponent->Zoom(false);
		}
	}
}