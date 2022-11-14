// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	virtual void StartFire();
	virtual void StopFire();
	virtual void NextWeapon();
	void Reload();

	bool GetWeaponUIData(FWeaponUIData& WeaponUIData) const;
	bool GetAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType);

	void Zoom(bool Enabled);

protected:
	int32 CurrentWeaponIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;
	UPROPERTY()
	TArray<ASTUBaseWeapon*> Weapons;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;
	void EquipWeapon(int32 WeaponIndex);


private:
	
	UPROPERTY()
	UAnimMontage* CurrentReloadAM = nullptr;

	bool bEquipAnimInProgress = false;
	bool bReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttackWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void PlayAnimMontage(UAnimMontage* AnimMontage);

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	void InitAnimations();

	void OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();
};
