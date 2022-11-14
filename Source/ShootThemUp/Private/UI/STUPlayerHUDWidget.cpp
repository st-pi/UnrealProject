// Shoot Them Up Game, All Rights Reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/ProgressBar.h"
#include "STUUtils.h"
#include "Player/STUPlayerState.h"

bool USTUPlayerHUDWidget::Initialize()
{
	if (auto OwningPlayer = GetOwningPlayer())
	{
		OwningPlayer->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
		OnNewPawn(OwningPlayer->GetPawn());
	}
	UpdateHealthBar();
	return Super::Initialize();
}

void USTUPlayerHUDWidget::OnDamageTaken(float Health)
{
	OnTakeDamage();
	if (!IsAnimationPlaying(DamageAnimation))
	{
		PlayAnimation(DamageAnimation);
	}
	UpdateHealthBar();
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
	if (const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn()))
	{
		return HealthComponent->GetHealthPercent();
	}
	return 0.0f;
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if (const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()))
	{
		return WeaponComponent->GetWeaponUIData(WeaponUIData);
	}
	return false;
}


bool USTUPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& WeaponAmmoData) const
{
	if (const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()))
	{
		return WeaponComponent->GetAmmoData(WeaponAmmoData);
	}
	return false;
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
	if (const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(NewPawn))
	{
		if (!HealthComponent->OnHealthChanged.IsBoundToObject(this))
		{
			HealthComponent->OnDamageTaken.AddUObject(this, &USTUPlayerHUDWidget::OnDamageTaken);
		}
	}
	UpdateHealthBar();
}

int32 USTUPlayerHUDWidget::GetKillsNum() const
{
	if (const auto Controller = GetOwningPlayer())
	{
		if (const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState))
		{
			return PlayerState->GetKillsNum();
		}
	}
	return 0;
}
int32 USTUPlayerHUDWidget::GetDeathsNum() const
{
	if (const auto Controller = GetOwningPlayer())
	{
		if (const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState))
		{
			return PlayerState->GetDeathsNum();
		}
	}
	return 0;
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
	if (HealthProgressBar)
	{
		const auto HealthBarColor = GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor;
		HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);
	}
}

FString USTUPlayerHUDWidget::FormatlBullets(int32 BulletsNum) const
{
	const int32 MaxLen = 3;
	const TCHAR PrefixSymbol = '0';

	auto BulletStr = FString::FromInt(BulletsNum);
	const auto SymbolsNumToAdd = MaxLen - BulletStr.Len();

	if (SymbolsNumToAdd > 0)
	{
		BulletStr = FString::ChrN(SymbolsNumToAdd, PrefixSymbol).Append(BulletStr);
	}
	return BulletStr;
}