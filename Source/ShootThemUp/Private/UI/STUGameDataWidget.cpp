// Shoot Them Up Game, All Rights Reserved


#include "UI/STUGameDataWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

int32 USTUGameDataWidget::GetCurrentRoundNum() const
{
	if (const auto GameMode = GetGameMode())
	{
		return GameMode->GetCurrentRoundNum();
	}
	return 0;
}
int32 USTUGameDataWidget::GetTotalRoundsNum() const
{
	if (const auto GameMode = GetGameMode())
	{
		return GameMode->GetGamaData().RoundsNum;
	}
	return 0;
}
int32 USTUGameDataWidget::GetTimeleft() const
{
	if (const auto GameMode = GetGameMode())
	{
		return GameMode->GetTimeleft();
	}
	return 0;
}
ASTUGameModeBase* USTUGameDataWidget::GetGameMode() const
{
	if (const auto World = GetWorld())
	{
		return Cast<ASTUGameModeBase>(World->GetAuthGameMode());
	}
	return nullptr;
}
ASTUPlayerState* USTUGameDataWidget::GetPlayerState() const
{
	if (const auto OwningPlayer = GetOwningPlayer())
	{
		return Cast<ASTUPlayerState>(OwningPlayer->PlayerState);
	}
	return nullptr;
}