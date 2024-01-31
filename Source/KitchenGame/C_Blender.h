// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Blender.generated.h"

UCLASS()
class KITCHENGAME_API AC_Blender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AC_Blender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UStaticMeshComponent> Cube;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
