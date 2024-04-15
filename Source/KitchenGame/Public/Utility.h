// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

/**
 * 
 */
class KITCHENGAME_API Utility
{
public:
	Utility();
	~Utility();
	
	static void MoveToTransform(USceneComponent* ToMove, USceneComponent* Origin, USceneComponent* Destination, bool NoRotation);
};
