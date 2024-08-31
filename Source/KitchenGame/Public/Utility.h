// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AIngredient.h"
#include "RecipeDataAsset.h"
#include "Utility.generated.h"
/**
 * 
 */
UCLASS()
class KITCHENGAME_API UUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UUtility();
	~UUtility();

	UFUNCTION(BlueprintCallable)
	static void MoveToTransform(USceneComponent* ToMove, USceneComponent* Origin, USceneComponent* Destination, bool NoRotation);

	UFUNCTION(BlueprintCallable)
	static FTransform MoveToTransform2(USceneComponent* ToMove, FTransform Origin, FTransform Destination, bool NoRotation);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector2D ProjectWorldToScreen(APlayerController const* Player, FVector WorldPosition, float& ArrowAngle);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString WithCapital(const FString& SInput);

	UFUNCTION(BlueprintCallable)
	static void DestroyActorAndAttached(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void GetActorAndAttached(TArray<AActor*>& OutActors, AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ESecondaryCookPhase GetRandomCookPhase();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ESauceType GetRandomSauceType();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int SumScoreParts(TArray<FScorePart> Parts);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int SumRecipeScore(TArray<FRecipeScore> Parts);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FScorePart> ConsolidateRecipeScore(const TArray<FScorePart>& Parts);

	UFUNCTION(BlueprintCallable)
	static void LaunchActor(AActor* Actor, FVector Velocity, bool bAddToCurrent);
};
