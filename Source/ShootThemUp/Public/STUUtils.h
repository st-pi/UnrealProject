#pragma once
#include "Player/STUPlayerState.h"

class STUUtils
{
public:
	template<typename T>
	static T* GetSTUPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn)
		{
			return nullptr;
		}
		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}

	bool static AreEnemies(AController* AC1, AController* AC2)
	{
		if (!AC1 || !AC2 || AC1 == AC2)
		{
			return false;
		}
		const auto PlayerState1 = Cast<ASTUPlayerState>(AC1->PlayerState);
		const auto PlayerState2 = Cast<ASTUPlayerState>(AC2->PlayerState);;
		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}
};