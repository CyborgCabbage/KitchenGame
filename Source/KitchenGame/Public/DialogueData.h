// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueData.generated.h"

USTRUCT(BlueprintType)
struct KITCHENGAME_API FDialogueData
{
	GENERATED_USTRUCT_BODY()

	FDialogueData();
	FDialogueData(FText Text, float WaitTime, bool LockPlayer, bool AutoAdvance, TObjectPtr<AActor> Target, bool Override);

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTime;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LockPlayer;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AutoAdvance;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> Target;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Override;
};