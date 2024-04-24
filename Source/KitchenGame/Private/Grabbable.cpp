// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabbable.h"
#include "LockPointTrigger.h"

// Sets default values for this component's properties
UGrabbable::UGrabbable() : CanGrab(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabbable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UGrabbable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrabbable::ConfigureGrabbable(UPrimitiveComponent* Physics, bool InHandValue) {
	InHand = InHandValue;
	GrabTarget = Physics;
	Physics->SetSimulatePhysics(true);
	for(UPrimitiveComponent* pc : TInlineComponentArray<UPrimitiveComponent*>{ Physics->GetOwner() }) {
		pc->CanCharacterStepUpOn = ECB_No;
		pc->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void UGrabbable::SetEnablePhysics(bool Simulating, bool Colliding) {
	GrabTarget->SetSimulatePhysics(Simulating);
	SetEnableCollision(Colliding, true);
}

void UGrabbable::UnlockIfLocked() {
	if(!IsLocked) return;
	if(!IsValid(LockPoint)) return;
	LockPoint->UnlockItem();
}

void UGrabbable::SetEnableCollision(bool Value, bool Recursive) {
	GrabTarget->SetCollisionEnabled(Value ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::QueryOnly);
	GetOwner()->ForEachAttachedActors([&] (AActor* attached) -> bool {
		if(auto* lockPointTrigger = Cast<ALockPointTrigger>(attached)) {
			lockPointTrigger->Trigger->SetCollisionEnabled(Value ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
		}
		return true;
	});
	if(Recursive) {
		GetOwner()->ForEachAttachedActors([&](AActor* attached) -> bool {
			if(auto* grabbable = attached->GetComponentByClass<UGrabbable>()) {
				grabbable->SetEnableCollision(Value, Recursive);
			}
			return true;
		});
	}
}

void UGrabbable::SetCanGrab(bool Value, bool Recursive) {
	CanGrab = Value;
	if (Recursive) {
		GetOwner()->ForEachAttachedActors([&](AActor* attached) -> bool {
			if (auto* grabbable = attached->GetComponentByClass<UGrabbable>()) {
				grabbable->SetCanGrab(Value, true);
			}
			return true;
		});
	}
}

