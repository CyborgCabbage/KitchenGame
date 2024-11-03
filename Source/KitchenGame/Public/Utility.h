// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AIngredient.h"
#include "RecipeDataAsset.h"
#include "Utility.generated.h"

UENUM(BlueprintType)
enum class ESnapDirection : uint8 {
	Up,
	Right,
	Down,
	Left
};

UENUM(BlueprintType)
enum class EStackRelation : uint8 {
	None,
	Above,
	Below,
	Same
};

USTRUCT(BlueprintType, Blueprintable)
struct FStackRelation {
	GENERATED_USTRUCT_BODY()
public:
	EStackRelation Relation = EStackRelation::None;
	bool Direct = false;
};


class UGrabbable;

USTRUCT(BlueprintType, Blueprintable)
struct FSnapMarkerInfo {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FVector2D PositionOnScreen = FVector2D::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	FVector WorldPosition = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	UGrabbable* Grabbable = nullptr;
	float DistanceAlongAxis = INFINITY;
};

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

	UFUNCTION(BlueprintCallable)
	static TMap<ESnapDirection, FSnapMarkerInfo> CalculateSnapMarkers(APlayerController const* Player, const TSet<UActorComponent*> &ComponentsInRange, AActor* SelectedActor, float ScreenBorder = 0.1f);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector2D GetSnapMarkerPositionOnScreen(APlayerController const* Player, const FSnapMarkerInfo& SnapMarkerInfo);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<AActor*> GetActorsInStack(AActor* Initial);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FStackRelation GetStackRelation(AActor* Base, AActor* Other);

};
