// Shoot Them Up Game, All Rights Reserved


#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

// Sets default values for this component's properties
USTURespawnComponent::USTURespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
	if (const auto World = GetWorld())
	{
		RespawnCountDown = RespawnTime;
		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
	}
}

void USTURespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown == 0)
	{
		if (const auto World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(RespawnTimerHandle);
			if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
			{
				GameMode->RespawnRequest(Cast<AController>(GetOwner()));
			}
		}
	}
}


int32 USTURespawnComponent::GetRespawnCountDown() const
{
	return RespawnCountDown;
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
	if (const auto World = GetWorld())
	{
		return World->GetTimerManager().IsTimerActive(RespawnTimerHandle);
	}
	return false;
}