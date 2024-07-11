// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IngredientDataAsset.h"
#include "AIngredient.generated.h"

UENUM(BlueprintType)
enum class ESauceType : uint8
{
	None,
	Tomato,
	Hot,
	Slime
};

UENUM(BlueprintType)
enum class EPrimaryCookPhase : uint8
{
	Raw,
	Frying,
	Fried,
	OverFried,
	Cooking,
	Cooked,
	Burnt
};

UENUM(BlueprintType)
enum class ESecondaryCookPhase : uint8
{
	Other,
	Rare,
	Medium,
	WellDone
};

USTRUCT(BlueprintType)
struct FFullCookPhase 
{
	GENERATED_USTRUCT_BODY()

	FFullCookPhase(EPrimaryCookPhase Primary = EPrimaryCookPhase::Raw, ESecondaryCookPhase Secondary = ESecondaryCookPhase::Other);

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPrimaryCookPhase Primary;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESecondaryCookPhase Secondary;
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ingredient State")
	TObjectPtr<UIngredientDataAsset> Data;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Recipe)
	FIngredientStatus GetStatus() const;

	UFUNCTION(BlueprintCallable, Category = Recipe)
	FFullCookPhase GetPhase() const;

};