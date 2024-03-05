#include "StockManager.h"
#include "Kismet/GameplayStatics.h"

FStockQuota::FStockQuota() :
	ActorClass(),
	Amount(0)
{
}

// Sets default values
AStockManager::AStockManager()
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
	for (const FStockQuota& quota : StockQuotas) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), quota.ActorClass, FoundActors);
		for (int i = FoundActors.Num(); i < quota.Amount; i++) {
			GetWorld()->SpawnActor(*quota.ActorClass, &GetTransform(), Parameters);
		}
	}

}

// Called every frame
void AStockManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

