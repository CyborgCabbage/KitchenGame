// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockPoint.h"
#include "Grabbable.generated.h"

class UGrabber;

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
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ConfigureGrabbable(UPrimitiveComponent* Physics, bool InHand, USceneComponent* BottomPoint, USceneComponent* GrabPoint);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetEnablePhysics(bool Simulating, bool Colliding);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UnlockIfLocked();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetEnableCollision(bool Value, bool Recursive);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetCanGrab(bool Value);

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<UPrimitiveComponent> GrabTarget;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool InHand;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> BottomPoint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> GrabPoint;

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
	TObjectPtr<AActor> Grabber;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool CanGrab;

};
