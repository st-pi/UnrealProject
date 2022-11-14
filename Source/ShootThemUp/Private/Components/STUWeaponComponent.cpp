// Shoot Them Up Game, All Rights Reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadAnimNotify.h"
#include "Animations/AnimUtils.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
	// ...
	
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto& Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::StartFire()
{
	if (CanFire())
	{
		CurrentWeapon->StartFire();
	}
}

void USTUWeaponComponent::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void USTUWeaponComponent::SpawnWeapons()
{
	auto World = GetWorld();
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!World || !Character)
	{
		return;
	}
	for (auto& Data : WeaponData)
	{
		auto Weapon = World->SpawnActor<ASTUBaseWeapon>(Data.WeaponClass);
		if (!Weapon)
		{
			continue;
		}
		Weapon->SetOwner(Character);
		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapons.Add(Weapon);

		AttackWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::AttackWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent)
	{
		return;
	}
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		return;
	}
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		CurrentWeapon->Zoom(false);
		AttackWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}
	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAM = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {return Data.WeaponClass == CurrentWeapon->GetClass(); });
	CurrentReloadAM = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttackWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
	PlayAnimMontage(EquipAnimMontage);
	bEquipAnimInProgress = true;
}

void USTUWeaponComponent::NextWeapon()
{
	if (CanEquip())
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
		EquipWeapon(CurrentWeaponIndex);
	}
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}
	Character->PlayAnimMontage(AnimMontage);
}

void USTUWeaponComponent::InitAnimations()
{
	if (auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage))
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}
	for (auto& Data : WeaponData)
	{
		if (auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadAnimNotify>(Data.ReloadAnimMontage))
		{
			ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
		}
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->GetMesh() == MeshComp)
	{
		bEquipAnimInProgress = false;
	}
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->GetMesh() == MeshComp)
	{
		bReloadAnimInProgress = false;
	}
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
	return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload()
{
	ChangeClip();
}

void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (const auto& Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void USTUWeaponComponent::ChangeClip()
{
	if (!CanReload())
	{
		return;
	}
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	PlayAnimMontage(CurrentReloadAM);
	bReloadAnimInProgress = true;
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& WeaponUIData) const
{
	if (CurrentWeapon)
	{
		WeaponUIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto& Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return true;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
	for (const auto& Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return true;
}

void USTUWeaponComponent::Zoom(bool Enabled)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}