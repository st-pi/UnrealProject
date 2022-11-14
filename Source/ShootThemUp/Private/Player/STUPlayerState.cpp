// Shoot Them Up Game, All Rights Reserved


#include "Player/STUPlayerState.h"


void ASTUPlayerState::SetTeamID(int32 ID)
{
	TeamID = ID;
}

int32 ASTUPlayerState::GetTeamID() const
{
	return TeamID;
}

void ASTUPlayerState::SetTeamColor(const FLinearColor& Color)
{
	TeamColor = Color;
}

FLinearColor ASTUPlayerState::GetTeamColor() const
{
	return TeamColor;
}

void ASTUPlayerState::AddKill()
{
	KillsNum++;
}

int32 ASTUPlayerState::GetKillsNum() const
{
	return KillsNum;
}

void ASTUPlayerState::AddDeath()
{
	DeathsNum++;
}

int32 ASTUPlayerState::GetDeathsNum() const
{
	return DeathsNum;
}
