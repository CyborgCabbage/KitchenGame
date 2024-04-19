// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector2D ProjectWorldToScreen(APlayerController const* Player, FVector WorldPosition, float& ArrowAngle);
};
