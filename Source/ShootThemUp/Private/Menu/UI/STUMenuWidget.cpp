// Shoot Them Up Game, All Rights Reserved


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "STUGameInstance.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Sound/SoundCue.h"

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}

	InitLevelItems();
}

void USTUMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTUMenuWidget::InitLevelItems()
{
	if (const auto GameInstance = GetGameInstance())
	{
		if (LevelItemsBox)
		{
			LevelItemsBox->ClearChildren();

			for (auto LevelData : GameInstance->GetLevelsData())
			{
				const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
				if (!LevelItemWidget)
				{
					continue;
				}

				LevelItemWidget->SetLevelData(LevelData);
				LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

				LevelItemsBox->AddChild(LevelItemWidget);
				LevelItemWidgets.Add(LevelItemWidget);
			}
		}
		if (GameInstance->GetStartupLevel().LevelName.IsNone())
		{
			OnLevelSelected(GameInstance->GetLevelsData()[0]);
		}
		else
		{
			OnLevelSelected(GameInstance->GetStartupLevel());
		}
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	if (const auto GameInstance = GetGameInstance())
	{
		GameInstance->SetStartupLevel(Data);
	}

	for (auto LevelItemWidget : LevelItemWidgets)
	{
		LevelItemWidget->SetSelected(Data.LevelName == LevelItemWidget->GetLevelData().LevelName);
	}

}

USTUGameInstance* USTUMenuWidget::GetGameInstance() const
{
	if (const auto World = GetWorld())
	{
		return World->GetGameInstance<USTUGameInstance>();
	}
	return nullptr;
}


void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == HideAnimation)
	{
		if (const auto GameInstance = GetGameInstance())
		{
			UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevel().LevelName);
		}
	}
}
