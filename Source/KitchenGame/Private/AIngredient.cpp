// Fill out your copyright notice in the Description page of Project Settings.

#include "AIngredient.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AIngredient::AIngredient() :
	CookAmount(0),
	FryAmount(0),
	SauceType(ESauceType::None),
	Data(nullptr)
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
	float Cooked = FMath::Clamp(CookAmount / Data->CookTime, 0, 1);
	float PreciseCooked = FMath::Clamp((CookAmount - Data->CookTime) / Data->CookedTime * 3, 0, 3);
	float Burnt = FMath::Clamp((CookAmount - Data->CookTime - Data->CookedTime) / Data->BurnTime, 0, 1);
	float Fried = FMath::Clamp(FryAmount / Data->FryTime, 0, 1);
	float OverFried = FMath::Clamp((FryAmount - Data->FryTime - Data->FriedTime) / Data->OverFryTime, 0, 1);
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
		status.ColorA = Data->CookColor;
		status.ColorB = Data->BurnColor;
		status.Progress = Burnt;
	}
	else if(OverFried > 0) {
		status.ColorA = Data->FryColor;
		status.ColorB = Data->OverFryColor;
		status.Progress = OverFried;
	}
	else if (Data->PreciseCooking && PreciseCooked > 0) {
		if (PreciseCooked < 1) {
			//Rare
			status.ColorA = FMath::Lerp(Data->RawColor, Data->CookColor, 1.0f / 4.0f);
			status.ColorB = FMath::Lerp(Data->RawColor, Data->CookColor, 2.0f / 4.0f);
			status.Progress = PreciseCooked;
		}
		else if (PreciseCooked < 2) {
			//Medium Rare
			status.ColorA = FMath::Lerp(Data->RawColor, Data->CookColor, 2.0f / 4.0f);
			status.ColorB = FMath::Lerp(Data->RawColor, Data->CookColor, 3.0f / 4.0f);
			status.Progress = PreciseCooked - 1.0f;
		}
		else {
			//Well Done
			status.ColorA = FMath::Lerp(Data->RawColor, Data->CookColor, 3.0f / 4.0f);
			status.ColorB = Data->CookColor;
			status.Progress = PreciseCooked - 2.0f;
		}
	}
	else if (Cooked > 0 && Cooked > Fried) {
		status.ColorA = Data->RawColor;
		status.ColorB = Data->PreciseCooking ? FMath::Lerp(Data->RawColor, Data->CookColor, 1.0f / 4.0f) : Data->CookColor;
		status.Progress = Cooked;
	}
	else if (Fried > 0) {
		status.ColorA = Data->RawColor;
		status.ColorB = Data->FryColor;
		status.Progress = Fried;
	}
	else {
		status.ColorA = Data->RawColor;
		status.ColorB = Data->RawColor;
		status.Progress = 0.0f;
	}
	return status;
}

FFullCookPhase AIngredient::GetPhase() const
{
	FFullCookPhase FCP;
	if (CookAmount > Data->CookTime + Data->CookedTime) {
		return { EPrimaryCookPhase::Burnt };
	}
	if (FryAmount > Data->FryTime + Data->FriedTime) {
		return { EPrimaryCookPhase::OverFried };
	}
	if (CookAmount > Data->CookTime) {
		double Mapped = FMath::GetMappedRangeValueClamped(TRange<double>{ Data->CookTime, Data->CookTime + Data->CookedTime }, TRange<double>{0, 3}, CookAmount);
		if(Mapped < 1) {
			return { EPrimaryCookPhase::Cooked, ESecondaryCookPhase::Rare };
		}
		else if (Mapped < 2) {
			return { EPrimaryCookPhase::Cooked, ESecondaryCookPhase::Medium };
		}
		else {
			return { EPrimaryCookPhase::Cooked, ESecondaryCookPhase::WellDone };
		}
	}
	if (FryAmount > Data->FryTime) {
		return { EPrimaryCookPhase::Fried };
	}
	if (CookAmount > 0) {
		return { EPrimaryCookPhase::Cooking };
	}
	if (FryAmount > 0) {
		return { EPrimaryCookPhase::Frying };
	}
	return {};
}

FIngredientStatus::FIngredientStatus() :
	ColorA(FColor::Black),
	ColorB(FColor::White),
	Progress(0.5f),
	SauceColor(FColor::Magenta),
	SauceProgress(1.0f)
{
}

FFullCookPhase::FFullCookPhase(EPrimaryCookPhase Primary, ESecondaryCookPhase Secondary) :
	Primary(Primary),
	Secondary(Secondary)
{
}
