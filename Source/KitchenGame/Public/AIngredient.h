// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIngredient.generated.h"

UCLASS(Blueprintable, BlueprintType)
class KITCHENGAME_API AIngredient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** How well done this ingredient is, a value over 1 means burnt */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double DoneAmount;

	/** How well fried this is, a value over 1 means over-fried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double FryAmount;

	/** How long it takes to go from 0 to 1 in doneness */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double DoneTime;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double BurnTime;

	/** How long it takes to go from 0 to 1 in friedness */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double FryTime;

	/** How long it takes to over-fry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double OverFryTime;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};