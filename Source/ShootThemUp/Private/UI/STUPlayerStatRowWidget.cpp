// Shoot Them Up Game, All Rights Reserved


#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (PlayerNameTextBlock)
	{
		PlayerNameTextBlock->SetText(Text);
	}
}
void USTUPlayerStatRowWidget::SetKills(const FText& Text)
{
	if (KillsTextBlock)
	{
		KillsTextBlock->SetText(Text);
	}
}
void USTUPlayerStatRowWidget::SetDeaths(const FText& Text) 
{
	if (DeathsTextBlock)
	{
		DeathsTextBlock->SetText(Text);
	}
}
void USTUPlayerStatRowWidget::SetTeam(const FText& Text)
{
	if (TeamTextBlock)
	{
		TeamTextBlock->SetText(Text);
	}
}
void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	if (PlayerIndicatorImage)
	{
		PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if (TeamImage)
	{
		TeamImage->SetColorAndOpacity(Color);
	}
}