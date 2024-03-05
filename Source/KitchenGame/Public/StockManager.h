// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StockManager.generated.h"

USTRUCT(BlueprintType)
struct FStockQuota
{
	GENERATED_USTRUCT_BODY()

	FStockQuota();

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Amount;
};

UCLASS(Blueprintable)
class KITCHENGAME_API AStockManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStockManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Restock();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly)
	TArray<FStockQuota> StockQuotas;
};
