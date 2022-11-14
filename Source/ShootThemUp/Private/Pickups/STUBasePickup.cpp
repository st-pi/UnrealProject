// Shoot Them Up Game, All Rights Reserved


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All)

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateRotationYawn();
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationYawn, 0.0f));
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return false;
}

bool ASTUBasePickup::CouldBeTaken() const
{
	return GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (const auto Pawn = Cast<APawn>(OtherActor))
	{
		if (GivePickupTo(Pawn))
		{
			PickedUp();
		}
	}
}

void ASTUBasePickup::PickedUp()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetRootComponent()->SetVisibility(false, true);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupTakenSound, GetActorLocation());
}

void ASTUBasePickup::Respawn()
{
	GenerateRotationYawn();
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetRootComponent()->SetVisibility(true, true);
}

void ASTUBasePickup::GenerateRotationYawn()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
	RotationYawn = FMath::RandRange(1.0f, 2.0f) * Direction;
}