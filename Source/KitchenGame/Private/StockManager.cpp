#include "StockManager.h"
#include "Kismet/GameplayStatics.h"

FStockQuota::FStockQuota() :
	ActorClass(),
	Amount(0)
{
}

// Sets default values
AStockManager::AStockManager() : 
	SpawnDisplacement({0, 0, 50})
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStockManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStockManager::Restock()
{
	FVector SpawnLocation = GetActorLocation();
	FActorSpawnParameters Parameters = {};
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FTransform SpawnTransform = GetTransform();
	SpawnTransform.SetScale3D({1, 1, 1});
	for (const FStockQuota& quota : StockQuotas) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), quota.ActorClass, FoundActors);
		for (int i = FoundActors.Num(); i < quota.Amount; i++) {
			GetWorld()->SpawnActor(*quota.ActorClass, &SpawnTransform, Parameters);
			SpawnTransform.SetLocation(SpawnTransform.GetLocation() + SpawnDisplacement);
		}
	}

}

// Called every frame
void AStockManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

