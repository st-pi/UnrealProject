// Shoot Them Up Game, All Rights Reserved


#include "AI/Services/STUFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/STUWeaponComponent.h"

USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	const auto bHasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
	if (Controller)
	{
		if (const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>())
		{
			if (Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName))
			{
				WeaponComponent->StartFire();
			}
			else
			{
				WeaponComponent->StopFire();
			}
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
