// Shoot Them Up Game, All Rights Reserved


#include "STUGameInstance.h"
#include "Sound/STUSoundFunctionLibrary.h"

FLevelData USTUGameInstance::GetStartupLevel() const
{
	return StartupLevel;
}

void USTUGameInstance::SetStartupLevel(const FLevelData& Data)
{
	StartupLevel = Data;
}

const TArray<FLevelData>& USTUGameInstance::GetLevelsData() const
{
	return LevelsData;
}

FName USTUGameInstance::GetMenuLevelName()
{
	return MenuLevelName;
}

void USTUGameInstance::ToggleVolume()
{
	USTUSoundFunctionLibrary::ToggleSoundClassVolume(MasteSoundClass);
}