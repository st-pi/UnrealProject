// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float RespawnTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* PickupTakenSound;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool GivePickupTo(APawn* PlayerPawn);
	bool CouldBeTaken() const;
private:
	float RotationYawn = 0.0f;
	FTimerHandle RespawnTimerHandle;

	void PickedUp();
	void Respawn();
	void GenerateRotationYawn();
};
