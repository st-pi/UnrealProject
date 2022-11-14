// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASTUGameModeBase();

	FOnMatchStateChagnedSignature OnMatchStateChanged;

	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

	void Killed(AController* Killer, AController* Victim);

	FGameData GetGamaData() const;
	int32 GetCurrentRoundNum() const;
	int32 GetTimeleft() const;

	void RespawnRequest(AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;

private:
	ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundTimerHandle;

	void SpawnBots();
	void StartRound();
	void UpdateGameTimer();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);

	void CreateTeams();
	FLinearColor GetColorByTeamID(int32 TeamID) const;
	void SetPlayerColor(AController* Controller);
	void StartRespawn(AController* Controller);

	void GameOver();
	void StopAllFire();

	void SetMatchState(ESTUMatchState State);
};
