// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockPoint.h"
#include "Grabbable.generated.h"

class UGrabber;

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnLockPointBeginOverlapSignature, ULockPoint*, LockPoint);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLockPointEndOverlapSignature, ULockPoint*, LockPoint);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnGrabbedSignature, UGrabbable*, Grabbable, UGrabber*, Grabber);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KITCHENGAME_API UGrabbable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabbable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FVector ReleaseLocation;
	FVector ReleaseDirection;
	float ReleaseTime;
	bool WasGrabbed;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ConfigureGrabbable(UPrimitiveComponent* Physics, bool InHand);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetEnablePhysics(bool Simulating, bool Colliding);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UnlockIfLocked();

	UFUNCTION(BlueprintCallable)
	void UngrabIfGrabbed();

	UFUNCTION(BlueprintCallable)
	void UnlockAndUngrab();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetEnableCollision(bool Value, bool Recursive);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetCanGrab(bool Value, bool Recursive);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOnClass(TSubclassOf<AActor> Class, bool Recursive);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsValidTrickShot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsValidDisposal() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetCenterOfMass() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UGrabbable* GetGrabbableBelow() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UGrabbable* GetGrabbableAbove() const;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UPrimitiveComponent> GrabTarget;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool InHand;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool IsLocked;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<ULockPoint> LockPoint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool IsGrabbed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Default")
	TObjectPtr<UGrabber> Grabber;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool CanGrab;

	UPROPERTY(BlueprintReadWrite)
	FOnLockPointBeginOverlapSignature OnLockPointBeginOverlapDelegate;

	UPROPERTY(BlueprintReadWrite)
	FOnLockPointEndOverlapSignature OnLockPointEndOverlapDelegate;

	UPROPERTY(BlueprintReadWrite)
	FOnGrabbedSignature OnGrabbedDelegate;
};
