// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChopRecipes.generated.h"

UENUM(BlueprintType)
enum class EChopResultType : uint8
{
	Single,
	Double
};

USTRUCT(BlueprintType)
struct FChopResult {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ResultClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EChopResultType Type;
};

/**
 * 
 */
UCLASS(BlueprintType)
class KITCHENGAME_API UChopRecipes : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<AActor>, FChopResult> ChopRecipes;
};
