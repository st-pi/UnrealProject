// Shoot Them Up Game, All Rights Reserved


#include "UI/STUGoToMenuWidget.h"
#include "Components/Button.h"
#include "STUGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USTUGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu);
	}
}

void USTUGoToMenuWidget::OnGoToMenu()
{
	if (const auto World = GetWorld())
	{
		if (const auto GameInstance = World->GetGameInstance<USTUGameInstance>())
		{
			if (!GameInstance->GetMenuLevelName().IsNone())
			{
				UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
			}
		}
	}
}