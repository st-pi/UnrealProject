// Shoot Them Up Game, All Rights Reserved


#include "UI/STUSpectatorWidget.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	if (const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>())
	{
		if (RespawnComponent->IsRespawnInProgress())
		{
			CountDownTime = RespawnComponent->GetRespawnCountDown();
			return true;
		}
	}
	return false;
}