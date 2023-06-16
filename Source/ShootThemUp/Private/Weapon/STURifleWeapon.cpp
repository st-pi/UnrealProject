// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ASTURifleWeapon::ASTURifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTURifleWeapon::StartFire()
{
	InitFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetFXActive(false);
}

void ASTURifleWeapon::MakeShot()
{
	const auto World = GetWorld();
	if (!World || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	FVector SocketLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		DealDamage(HitResult);
		TraceFXEnd = HitResult.ImpactPoint;
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	SpawnFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
	TraceStart = ViewLocation;
	const auto HalfRadius = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRadius);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURifleWeapon::DealDamage(const FHitResult& HitResult)
{
	if (const auto DamagedActor = HitResult.GetActor())
	{
		FPointDamageEvent PointDamageEvent;
		PointDamageEvent.HitInfo = HitResult;
		DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
	}
}

void ASTURifleWeapon::InitFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	if (!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMeshComponent, MuzzleSocketName);
	}
	SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}
	if (FireAudioComponent)
	{
		IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
	}
}

void ASTURifleWeapon::SpawnFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart))
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}