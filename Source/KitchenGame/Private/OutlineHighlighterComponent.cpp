// Fill out your copyright notice in the Description page of Project Settings.


#include "OutlineHighlighterComponent.h"

// Sets default values for this component's properties
UOutlineHighlighterComponent::UOutlineHighlighterComponent() : Targets(), Mode(EOutlineHighlightMode::HIGHLIGHT)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOutlineHighlighterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UOutlineHighlighterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	SetHighlighted(nullptr);
}


// Called every frame
void UOutlineHighlighterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOutlineHighlighterComponent::SetHighlighted(AActor* NewTarget) {
	TArray<AActor*> NewTargets;
	if(IsValid(NewTarget)) {
		NewTargets.Add(NewTarget);
		NewTarget->GetAttachedActors(NewTargets, false, true);
	}
	TArray<AActor*> ToRemove;
	for (AActor* Actor : Targets) {
		if (!NewTargets.Contains(Actor)) {
			ToRemove.Add(Actor);
		}
	}
	/*TArray<AActor*> ToAdd;
	for (AActor* Actor : NewTargets) {
		if (!Targets.Contains(Actor)) {
			ToAdd.Add(Actor);
		}
	}*/
	//Clear bit on old outline
	for(AActor* Actor : ToRemove) {
		TArray<UPrimitiveComponent*> Primitives;
		Actor->GetComponents(Primitives);
		for(UPrimitiveComponent* Primitive : Primitives) {
			if(Primitive->bRenderCustomDepth) {
				Primitive->SetCustomDepthStencilValue(Primitive->CustomDepthStencilValue & ~((uint32)1 << (uint8)Mode));
			}
		}
	}
	//Set bit for new outline
	for (AActor* Actor : /*ToAdd*/NewTargets) {
		TArray<UPrimitiveComponent*> Primitives;
		Actor->GetComponents(Primitives);
		for (UPrimitiveComponent* Primitive : Primitives) {
			if (Primitive->bRenderCustomDepth) {
				Primitive->SetCustomDepthStencilValue(Primitive->CustomDepthStencilValue | ((uint32)1 << (uint8)Mode));
			}
		}
	}
	Targets = NewTargets;
}

