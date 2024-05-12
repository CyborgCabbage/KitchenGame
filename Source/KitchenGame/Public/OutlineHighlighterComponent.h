// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OutlineHighlighterComponent.generated.h"

UENUM(BlueprintType)
enum class EOutlineHighlightMode : uint8 {
	HIGHLIGHT,
	TUTORIAL
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KITCHENGAME_API UOutlineHighlighterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOutlineHighlighterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	TArray<AActor*> Targets;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetHighlighted(AActor* NewTarget);

	UPROPERTY(EditDefaultsOnly)
	EOutlineHighlightMode Mode;
};
