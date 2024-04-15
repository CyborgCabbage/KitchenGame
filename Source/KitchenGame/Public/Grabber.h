// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

class UGrabbable;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KITCHENGAME_API UGrabber : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TObjectPtr<UGrabbable> Grabbed;
	TObjectPtr<USceneComponent> Hand;
	TOptional<FTransform> TargetLocation;
	bool AttachedToHand;
	TObjectPtr<UPhysicsHandleComponent> PhysicsHandle;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetHand(USceneComponent* SceneComponent);

	UFUNCTION(BlueprintCallable)
	void SetTargetTransform(FTransform Transform);

	UFUNCTION(BlueprintCallable)
	void TryPickup(UGrabbable* Grabbable);

	UFUNCTION(BlueprintCallable)
	void TryDrop();
};