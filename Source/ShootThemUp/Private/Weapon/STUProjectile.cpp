// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnHit);
	SetLifeSpan(LifeSpanSeconds);
}

void ASTUProjectile::SetShotDirection(const FVector& Direction)
{
	ShotDirection = Direction;
}

void ASTUProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto World = GetWorld();
	if (!World)
	{
		return;
	}
	MovementComponent->StopMovementImmediately();

	UGameplayStatics::ApplyRadialDamage(World, DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(), {GetOwner()}, this, GetController(), bDoFullDamage);
	WeaponFXComponent->PlayImpactFX(Hit);

	Destroy();
}

AController* ASTUProjectile::GetController()
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
