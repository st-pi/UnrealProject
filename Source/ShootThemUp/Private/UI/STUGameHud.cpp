// Shoot Them Up Game, All Rights Reserved


#include "UI/STUGameHud.h"
#include "UI/STUBaseWidget.h"
#include "STUGameModeBase.h"

void ASTUGameHud::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));
	for (auto WidgetPair : GameWidgets)
	{
		if (const auto GameWidget = WidgetPair.Value)
		{
			GameWidget->AddToViewport();
			GameWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (const auto World = GetWorld())
	{
		if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHud::OnMatchStateChanged);
		}
	}
}

void ASTUGameHud::OnMatchStateChanged(ESTUMatchState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}
}