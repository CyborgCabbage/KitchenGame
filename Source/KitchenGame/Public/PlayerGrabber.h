// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grabber.h"
#include "PlayerGrabber.generated.h"

class ALockPointTrigger;
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KITCHENGAME_API UPlayerGrabber : public UGrabber
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UPlayerGrabber();
protected:
	virtual void BeginPlay() override;
	virtual bool FinishPickup() override;
	virtual void FinishDrop() override;
private:
	TObjectPtr<USceneComponent> View;
	TObjectPtr<USceneComponent> Hand;
	bool AttachedToHand;
	UPROPERTY(EditDefaultsOnly)
	float DropDistance;
	UPROPERTY(EditDefaultsOnly)
	float GrabDistance;
	UPROPERTY(EditDefaultsOnly)
	float TraceDistance;
	UPROPERTY(EditDefaultsOnly)
	float TraceRadius;
	ALockPointTrigger* TraceLockPoint();
	ALockPointTrigger* OverlapLockPoint();

	ALockPointTrigger* GetTargetLockPoint();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TryDropToLockPoint();
};
