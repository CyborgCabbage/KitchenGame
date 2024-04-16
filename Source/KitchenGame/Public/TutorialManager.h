// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnTutorialStepSignature);

UCLASS(Blueprintable)
class KITCHENGAME_API ATutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//UPROPERTY(BlueprintAssignable)
	FOnTutorialStepSignature OnTutorialStepDelegate;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool IsActive;

	UFUNCTION(BlueprintCallable)
	void SetTutorialDelegate(UPARAM(DisplayName = "Event") FOnTutorialStepSignature Delegate);
};
