// Shoot Them Up Game, All Rights Reserved


#include "Player/STUBaseCharacter.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("STUHealthComponent");
	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
	OnHealthChanged(HealthComponent->GetHealth());

	LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASTUBaseCharacter::IsRunning() const
{
	return false;
} 

float ASTUBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
	{
		return 0.0f;
	}
	const auto VelocityNormalized = GetVelocity().GetSafeNormal();
	// arccos(scalar forward vector and velocity vector)
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormalized));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormalized);
	const auto Degress = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degress : Degress * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	if (const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
	{
		MaterialInst->SetVectorParameterValue(MaterialColorName, Color);
		TestColor = Color;
	}
}

void ASTUBaseCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& FHit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	if (FallVelocityZ >= LandedDamageVelocity.X)
	{
		const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
		TakeDamage(FinalDamage, FPointDamageEvent{}, nullptr, nullptr);
	}
}

void ASTUBaseCharacter::TurnOff()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	Super::TurnOff();
}

void ASTUBaseCharacter::Reset()
{
	WeaponComponent->StopFire();
	WeaponComponent->Zoom(false);
	Super::Reset();
}