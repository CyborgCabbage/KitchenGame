// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EIngredient : uint8 {
	EI_BREAD UMETA(DisplayName = "Bread"),
	EI_BREAD_SLICE UMETA(DisplayName = "Bread Slice"),
	EI_EYEBALL UMETA(DisplayName = "Eyeball"),
	EI_POTATO UMETA(DisplayName = "Potato"),
	EI_POTATO_CHIP UMETA(DisplayName = "Potato Chip"),
	EI_MILK UMETA(DisplayName = "Milk"),
	EI_LETTUCE UMETA(DisplayName = "Lettuce"),
	EI_TOMATO UMETA(DisplayName = "Tomato Slice"),
	EI_MINCE UMETA(DisplayName = "Mince"),
	EI_WING UMETA(DisplayName = "Wing"),
};