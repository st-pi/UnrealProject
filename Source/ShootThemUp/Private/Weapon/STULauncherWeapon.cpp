// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ASTULauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTULauncherWeapon::StopFire()
{

}

void ASTULauncherWeapon::MakeShot()
{
	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
	}

	const auto World = GetWorld();
	if (!World)
	{
		return;
	}
	FVector TraceStart, TraceEnd;
	if (GetTraceData(TraceStart, TraceEnd))
	{
		FHitResult HitResult;
		MakeHit(HitResult, TraceStart, TraceEnd);
		const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();
		const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
		if (ASTUProjectile* Projectile = World->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform))
		{
			Projectile->SetShotDirection(Direction);
			Projectile->SetOwner(GetOwner());
			Projectile->FinishSpawning(SpawnTransform);
		}

	}
	DecreaseAmmo();
	SpawnMuzzleFX();
	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMeshComponent, MuzzleSocketName);
}