// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

class USoundClass;

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FLevelData GetStartupLevel() const;
	void SetStartupLevel(const FLevelData& Data);

	const TArray<FLevelData>& GetLevelsData() const;

	FName GetMenuLevelName();

	void ToggleVolume();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName StartupLevelname = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundClass* MasteSoundClass;
private:
	FLevelData StartupLevel;
};
