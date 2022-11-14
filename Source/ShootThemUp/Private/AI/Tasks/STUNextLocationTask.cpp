// Shoot Them Up Game, All Rights Reserved


#include "AI/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
	NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard)
	{
		return EBTNodeResult::Type::Failed;
	}

	const auto Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Type::Failed;
	}
	const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSystem)
	{
		return EBTNodeResult::Type::Failed;
	}
	FNavLocation NavLocation;
	FVector Location = Pawn->GetActorLocation();
	if (!bSelfCenter)
	{
		if (auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName)))
		{
			Location = CenterActor->GetActorLocation();
		}
		else
		{
			return EBTNodeResult::Type::Failed;
		}
	}

	if (!NavSystem->GetRandomReachablePointInRadius(Location, Radius, NavLocation))
	{
		return EBTNodeResult::Type::Failed;
	}
	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Type::Succeeded;
}