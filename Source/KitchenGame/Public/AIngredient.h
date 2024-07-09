// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIngredient.generated.h"

UENUM(BlueprintType)
enum class ESauceType : uint8
{
	None,
	Tomato,
	Hot,
	Slime
};

USTRUCT(BlueprintType)
struct FIngredientStatus
{
	GENERATED_USTRUCT_BODY()
	
	FIngredientStatus();

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor ColorA;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor ColorB;
	
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Progress;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor SauceColor;

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SauceProgress;
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient State")
	double CookAmount;

	/** How fried is this ingredient */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient State")
	double FryAmount;
	
	/** The applied sauce type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient State")
	ESauceType SauceType;

	//Cooking Times
	
	/** How long it takes to cook */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	double CookTime;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	double CookedTime;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	double BurnTime;

	//Frying Times

	/** How long it takes to fry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	double FryTime;

	/** How long before it starts burning */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	double FriedTime;

	/** How long it takes to over fry */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	double OverFryTime;

	/** The ID of the ingredient */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient Config")
	FString IngredientID;

	/** How long it takes to burn */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ingredient Config")
	bool PreciseCooking;

	//Colors

	/** How long it takes to cook */
	UPROPERTY(EditDefaultsOnly, Category = "Ingredient Color Config")
	FLinearColor RawColor;

	/** How long before it starts burning */
	UPROPERTY(EditDefaultsOnly, Category = "Ingredient Color Config")
	FLinearColor CookColor;

	/** How long it takes to burn */
	UPROPERTY(EditDefaultsOnly, Category = "Ingredient Color Config")
	FLinearColor BurnColor;

	/** How long before it starts burning */
	UPROPERTY(EditDefaultsOnly, Category = "Ingredient Color Config")
	FLinearColor FryColor;

	/** How long it takes to burn */
	UPROPERTY(EditDefaultsOnly, Category = "Ingredient Color Config")
	FLinearColor OverFryColor;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Recipe)
	FIngredientStatus GetStatus() const;

	UFUNCTION(BlueprintCallable, Category = Recipe)
	FString GetPhase() const;

};