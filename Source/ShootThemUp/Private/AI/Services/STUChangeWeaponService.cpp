// Shoot Them Up Game, All Rights Reserved


#include "AI/Services/STUChangeWeaponService.h"
#include "Components/STUWeaponComponent.h"
#include "AIController.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const auto Controller = OwnerComp.GetAIOwner())
	{
		if (const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>())
		{
			if (Probability > 0 && FMath::Rand() <= Probability)
			{
				WeaponComponent->NextWeapon();
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}