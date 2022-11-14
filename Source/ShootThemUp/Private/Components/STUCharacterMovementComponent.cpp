// Shoot Them Up Game, All Rights Reserved


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
	if (Player && Player->IsRunning())
	{
		MaxSpeed *= RunModifier;
	}
	return MaxSpeed;
}