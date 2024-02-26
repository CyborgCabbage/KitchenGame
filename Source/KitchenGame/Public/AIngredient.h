// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIngredient.generated.h"

USTRUCT(BlueprintType)
struct FIngredientStatus
{
	GENERATED_USTRUCT_BODY()
	
	FIngredientStatus();

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cooked;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Fried;
	
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Burnt;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OverFried;
};

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
	//Properties
	
	/** How cooked is this ingredient */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double CookAmount;

	/** How fried is this ingredient */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double FryAmount;
	
	//Cooking Times
	
	/** How long it takes to cook */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double CookTime;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double CookedTime;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double BurnTime;

	//Frying Times

	/** How long it takes to fry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double FryTime;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double FriedTime;

	/** How long it takes to over fry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	double OverFryTime;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Recipe)
	FIngredientStatus GetStatus() const;

	UFUNCTION(BlueprintCallable, Category = Recipe)
	FString GetPhase() const;

};