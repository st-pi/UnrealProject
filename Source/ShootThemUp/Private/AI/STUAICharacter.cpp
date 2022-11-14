// Shoot Them Up Game, All Rights Reserved


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"


ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ASTUAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (auto CharMovement = GetCharacterMovement())
	{
		CharMovement->bUseControllerDesiredRotation = true;
		CharMovement->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASTUAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::OnHealthChanged(float Health)
{
	Super::OnHealthChanged(Health);

	if (const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
	}
}

void ASTUAICharacter::OnDeath()
{
	Super::OnDeath();
	if (const auto STUController = Cast<AAIController>(Controller))
	{
		if (STUController->BrainComponent)
		{
			STUController->BrainComponent->Cleanup();
		}
	}
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
	if (const auto World = GetWorld())
	{
		if (const auto FirstPlayerController = World->GetFirstPlayerController())
		{
			if (const auto Pawn = FirstPlayerController->GetPawnOrSpectator())
			{
				const auto PlayerLocation = Pawn->GetActorLocation();
				const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
				HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
			}
		}
	}
}