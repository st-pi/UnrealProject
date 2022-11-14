// Shoot Them Up Game, All Rights Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShake.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

float USTUHealthComponent::GetHealth() const
{
	return Health;
}

float USTUHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (!IsDead() && IsFullHealth())
	{
		SetHealth(Health + HealthAmount);
		return true;
	}
	return false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	if (AActor* ComponentOwner = GetOwner())
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
	}
	OnHealthChanged.Broadcast(Health);
}

bool USTUHealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);
	if (IsFullHealth())
	{
		if (auto World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(HealTimerHandle);
		}
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

bool USTUHealthComponent::IsFullHealth()
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}


void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead())
	{
		return;
	}

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player)
	{
		return;
	}

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager)
	{
		return;
	}
	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if (const auto World = GetWorld())
	{
		if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
		{
			const auto Player = Cast<APawn>(GetOwner());
			const auto VictimController = Player ? Player->Controller : nullptr;
			GameMode->Killed(KillerController, VictimController);
		}
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	UE_LOG(LogTemp, Display, TEXT("On point damage taken: %f bone: %s"), FinalDamage, *BoneName.ToString());
	ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	auto World = GetWorld();
	if (Damage <= 0.0f || IsDead() || !World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(HealTimerHandle);

	SetHealth(Health - Damage);
	OnDamageTaken.Broadcast(Damage);
	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		World->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if (Character && Character->GetMesh())
	{
		const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
		if (DamageModifiers.Contains(PhysMaterial))
		{
			return DamageModifiers[PhysMaterial];
		}
	}
	return 1.0f;
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner())
	{
		return;
	}
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}