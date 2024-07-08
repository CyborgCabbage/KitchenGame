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
	RawColor(1, 1, 1),
	CookColor(0.7f, 0.7f, 0.7f),
	BurnColor(0, 0, 0),
	FryColor(1.0f, 0.4f, 0.04f),
	OverFryColor(0.2f, 0.1f, 0),
	SauceType(ESauceType::None),
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
	float Cooked = FMath::Clamp(CookAmount / CookTime, 0, 1);
	float Burnt = FMath::Clamp((CookAmount - CookTime - CookedTime) / BurnTime, 0, 1);
	float Fried = FMath::Clamp(FryAmount / FryTime, 0, 1);
	float OverFried = FMath::Clamp((FryAmount - FryTime - FriedTime) / OverFryTime, 0, 1);
	FIngredientStatus status;
	switch (SauceType) {
	case ESauceType::None:
		status.SauceProgress = 0.0f;
		status.SauceColor = FLinearColor::Black;
		break;
	case ESauceType::Tomato:
		status.SauceProgress = 1.0f;
		status.SauceColor = { 1.0f, 0.0f, 0.0f };
		break;
	case ESauceType::Hot:
		status.SauceProgress = 1.0f;
		status.SauceColor = { 0.6f, 0.0f, 0.0f };
		break;
	case ESauceType::Slime:
		status.SauceProgress = 1.0f;
		status.SauceColor = { 0.7f, 1.0f, 0.7f };
		break;
	}
	if (Burnt > 0.0f && Burnt > OverFried) {
		status.ColorA = CookColor;
		status.ColorB = BurnColor;
		status.Progress = Burnt;
	}
	else if(OverFried > 0) {
		status.ColorA = FryColor;
		status.ColorB = OverFryColor;
		status.Progress = OverFried;
	}
	else if (Cooked > 0 && Cooked > Fried) {
		status.ColorA = RawColor;
		status.ColorB = CookColor;
		status.Progress = Cooked;
	}
	else if (Fried > 0) {
		status.ColorA = RawColor;
		status.ColorB = FryColor;
		status.Progress = Fried;
	}
	else {
		status.ColorA = RawColor;
		status.ColorB = RawColor;
		status.Progress = 0.0f;
	}
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
	ColorA(FColor::Black),
	ColorB(FColor::White),
	Progress(0.5f),
	SauceColor(FColor::Magenta),
	SauceProgress(1.0f)
{
}
