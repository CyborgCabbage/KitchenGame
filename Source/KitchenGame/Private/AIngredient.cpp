// Fill out your copyright notice in the Description page of Project Settings.

#include "AIngredient.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AIngredient::AIngredient() :
	CookAmount(0),
	FryAmount(0),
	CookTime(10),
	CookedTime(10),
	BurnTime(10),
	FryTime(10),
	FriedTime(10),
	OverFryTime(10),
	IngredientID("")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIngredient::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FIngredientStatus AIngredient::GetStatus() const
{
	FIngredientStatus status;
	status.Cooked = FMath::Clamp(CookAmount / CookTime, 0, 1);
	status.Burnt = FMath::Clamp((CookAmount - CookTime - CookedTime) / BurnTime, 0, 1);
	status.Fried = FMath::Clamp(FryAmount / FryTime, 0, 1);
	status.OverFried = FMath::Clamp((FryAmount - FryTime - FriedTime) / OverFryTime, 0, 1);
	return status;
}

FString AIngredient::GetPhase() const 
{
	if (CookAmount > CookTime + CookedTime) {
		return "burned";
	}
	if (FryAmount > FryTime + FriedTime) {
		return "overfried";
	}
	if (CookAmount > CookTime) {
		return "cooked";
	}
	if (FryAmount > FryTime) {
		return "fried";
	}
	if (CookAmount > 0) {
		return "cooking";
	}
	if (FryAmount > 0) {
		return "frying";
	}
	return "raw";
}

FIngredientStatus::FIngredientStatus() :
	Cooked(0),
	Fried(0),
	Burnt(0),
	OverFried(0)
{
}
