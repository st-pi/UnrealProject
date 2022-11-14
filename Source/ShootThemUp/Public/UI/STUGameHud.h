// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHud.generated.h"

class USTUBaseWidget;

UCLASS()
class SHOOTTHEMUP_API ASTUGameHud : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;
private:
	UPROPERTY()
	TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;

	UPROPERTY()
	USTUBaseWidget* CurrentWidget = nullptr;

	void OnMatchStateChanged(ESTUMatchState State);
};
