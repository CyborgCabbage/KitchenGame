// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LockPointTrigger.h"
#include "LockPoint.generated.h"

class UGrabbable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract, Blueprintable)
class KITCHENGAME_API ULockPoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockPoint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool Enabled);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void LockItem(UGrabbable* Grabbed);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UnlockItem();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	bool CanLock(UGrabbable* Grabbable);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	ALockPointTrigger* GetTriggerActor(bool& Enabled);
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockPoint")
	bool AllowStack;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockPoint")
	bool AlwaysEnabled;
protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category = "LockPoint")
	TObjectPtr<ALockPointTrigger> CurrentActor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category = "LockPoint")
	bool ActorEnabled;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LockPoint")
	TSubclassOf<class ALockPointTrigger>  TriggerClass;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LockPoint")
	bool InUse;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "LockPoint")
	TObjectPtr<UGrabbable> LockedItem;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LockPoint")
	TArray<UClass*> AllowedClasses;

};
