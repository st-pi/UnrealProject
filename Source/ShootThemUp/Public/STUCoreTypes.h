#pragma once

#include "STUCoreTypes.generated.h"

/* Weapon*/

class ASTUBaseWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ASTUBaseWeapon*);


USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool bInfinite = false;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UTexture2D* MainIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UTexture2D* CrosshairIcon;
};

/* Health */
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, float)

/* VFX */

class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FVector Size = FVector(10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float FadeOutTime = 0.7f;;
};

USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FDecalData DecalData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
		USoundCue* Sound;
};

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (Clampmin = "1", Clampmax = "100"))
	int32 PlayersNum = 32;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (Clampmin = "1", Clampmax = "10"))
	int32 RoundsNum = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (Clampmin = "3", Clampmax = "300"))
	int32 RoundTime = 60;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FLinearColor DefaultTeamColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game");
	TArray<FLinearColor> TeamColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (Clampmin = "3", Clampmax = "20"))
	int32 RespawnTime = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (Clampmin = "3", Clampmax = "20"))
	int32 MinTimeleftForRespawn = 3;
};

UENUM(BlueprintType)
enum class ESTUMatchState : uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	GameOver
};


DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChagnedSignature, ESTUMatchState);


USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game");
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game");
	FName LevelDisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game");
	UTexture2D* LevelIcon;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);
