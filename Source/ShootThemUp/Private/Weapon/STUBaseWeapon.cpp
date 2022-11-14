// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponMeshComponent = CreateDefaultSubobject< USkeletalMeshComponent>("WeaponMeshComponent");
	SetRootComponent(WeaponMeshComponent);
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = DefaultAmmo;
	
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::MakeShot()
{
}

const FWeaponUIData& ASTUBaseWeapon::GetUIData() const
{
	return UIData;
}

const FAmmoData& ASTUBaseWeapon::GetAmmoData() const
{
	return CurrentAmmo;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (IsAmmoFull())
	{
		return false;
	}

	if (IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}
	return true;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto STUCharacter = Cast<ACharacter>(GetOwner());
	if (!STUCharacter)
	{
		return false;
	}
	if (STUCharacter->IsPlayerControlled())
	{
		const auto Controller = STUCharacter->GetController<APlayerController>();
		if (!Controller)
		{
			return false;
		}
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
	}
	return true;
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
	{
		return false;
	}
	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto World = GetWorld();
	if (!World)
	{
		return;
	}
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

	World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}


void ASTUBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("No bullets"));
		return;
	}

	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}
bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.bInfinite || (CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets);
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}
void ASTUBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.bInfinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Display, TEXT("No clips"));
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("Reload clip"));
}
bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets&& CurrentAmmo.Clips > 0;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMeshComponent, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void ASTUBaseWeapon::Zoom(bool Enabled)
{

}