// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetDeathsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	FString FormatlBullets(int32 BulletsNum) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetWeaponAmmoData(FAmmoData& WeaponAmmoData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage();

	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* DamageAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentVisibilityThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor GoodColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor BadColor = FLinearColor::Red;

private:
	void OnDamageTaken(float Health);

	void OnNewPawn(APawn* NewPawn);
	void UpdateHealthBar();
};
