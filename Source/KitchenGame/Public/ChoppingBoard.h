// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockPoint.h"
#include "ChopRecipes.h"
#include "ChoppingBoard.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent), Abstract, Blueprintable)
class KITCHENGAME_API AChoppingBoard : public AActor
{
	GENERATED_BODY()
protected:
	ULockPoint* GetLockPoint();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* leftBread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* rightBread;
public:
	// Sets default values for this actor's properties
	AChoppingBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool CanChop(const AActor* target, FChopResult& result);

	UFUNCTION(BlueprintCallable)
	void TryChop();

	UFUNCTION(BlueprintImplementableEvent)
	void OnChop();

};
