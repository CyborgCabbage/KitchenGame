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
	
	float Speed = IsValid(GrabTarget) ? GrabTarget->GetComponentVelocity().Length() : 0.0f;
	if (!IsGrabbed && !IsLocked) {
		if (Speed > 5.0f) {
			AirTime += DeltaTime;
		}
		else if (Speed < 0.5f) {
			AirTime = 0;
		}
	}else{
		AirTime = 0;
	}
}

void UGrabbable::ConfigureGrabbable(UPrimitiveComponent* Physics, bool InHandValue) {
	InHand = InHandValue;
	GrabTarget = Physics;
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

bool UGrabbable::IsOnClass(TSubclassOf<AActor> Class, bool Recursive)
{
	auto* Parent = GetOwner()->GetAttachParentActor();
	if (!Parent) return false;
	if (Parent->IsA(Class)) return true;
	if (Recursive) {
		if (auto* Grabbable = Parent->GetComponentByClass<UGrabbable>()) {
			Grabbable->IsOnClass(Class, true);
		}
	}
	return false;
}

float UGrabbable::GetAirTime() const
{
	return AirTime;
}

