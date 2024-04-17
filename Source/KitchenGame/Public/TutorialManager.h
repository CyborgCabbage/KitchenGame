// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TutorialManager.generated.h"

class ATutorialManager;

/**
 * Signatures of execution pins in the editor
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTutorialExecPinSignature);

/**
 *
 */
UCLASS()
class UTutorialStep : public UBlueprintAsyncActionBase
{
	friend ATutorialManager;
	GENERATED_BODY()

private:
	ATutorialManager* TutorialManager;
	FText Instruction;
	bool Essential;
	bool Remember;
	bool Passed;
public:

	UPROPERTY(BlueprintAssignable)
	FTutorialExecPinSignature Tick;

	UPROPERTY(BlueprintAssignable)
	FTutorialExecPinSignature Check;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UTutorialStep* TutorialStep(ATutorialManager* TutorialManager, FText Instruction, bool Essential, bool Remember);

	virtual void Activate() override;

	bool IsPassed();
};

UCLASS(Blueprintable, Abstract)
class KITCHENGAME_API ATutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialManager();
private:
	TArray<TObjectPtr<UTutorialStep>> Steps;
	AActor* Highlighted;
	FText CurrentInstruction;
	int CurrentStep;
	void SetInstruction(const FText& Text);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddTutorialStep(UTutorialStep* Step);

	UFUNCTION(BlueprintCallable)
	void SetPassed(bool Value = true);

	UFUNCTION(BlueprintCallable)
	void SetPassedIfValid(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void SetHighlight(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsGrabbingClass(TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetGrabbingClass(TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> FilterPhase(const TArray<AActor*>& Actors, FString Phase);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void FilterAbove(const TArray<AActor*>& Actors, TSubclassOf<AActor> AboveClass, TArray<AActor*>& ActorsTop, TArray<AActor*>& ActorsBottom);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void FilterBelow(const TArray<AActor*>& Actors, TSubclassOf<AActor> BelowClass, TArray<AActor*>& ActorsTop, TArray<AActor*>& ActorsBottom);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetActorOnActor(TSubclassOf<AActor> Bottom, TSubclassOf<AActor> Top);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetNearestActor(TArray<AActor*> Actors);

	// Create Event
	UFUNCTION(BlueprintImplementableEvent)
	void OnInstructionChange(const FText& NewInstruction);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHighlightChange(const AActor* NewHighlight);

	UPROPERTY(BlueprintReadWrite)
	bool IsActive;
};
