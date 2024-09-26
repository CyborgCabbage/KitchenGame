// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabbable.h"
#include "LockPointTrigger.h"
#include "PlayerGrabber.h"

// Sets default values for this component's properties
UGrabbable::UGrabbable() : 
	ReleaseLocation(),
	ReleaseDirection(FVector::ForwardVector),
	ReleaseTime(INFINITY),
	WasGrabbed(false),
	CanGrab(true)
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

void UGrabbable::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{
	UnlockAndUngrab();
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UGrabbable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	auto* PlayerGrabber = Cast<UPlayerGrabber>(Grabber);
	if (IsGrabbed && PlayerGrabber) {
		ReleaseLocation = PlayerGrabber->GetView()->GetComponentLocation();
		ReleaseDirection = PlayerGrabber->GetView()->GetComponentRotation().Vector();
		ReleaseTime = 0.0f;
		WasGrabbed = true;
	}
	else if (IsGrabbed || IsLocked) {
		ReleaseTime = INFINITY;
		WasGrabbed = false;
	} 
	else if(WasGrabbed) {
		ReleaseTime += DeltaTime;
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

void UGrabbable::UngrabIfGrabbed() {
	if(!IsGrabbed) return;
	if(!IsValid(Grabber)) return;
	Grabber->TryDrop();
}

void UGrabbable::UnlockAndUngrab() {
	UnlockIfLocked();
	UngrabIfGrabbed();
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

bool UGrabbable::IsValidTrickShot() const
{
	if (!WasGrabbed) return false;
	if (ReleaseTime > 5.0f) return false;
	if (FVector::DistXY(ReleaseLocation, GrabTarget->GetComponentLocation()) < 400.0f) return false;
	return true;
}

bool UGrabbable::IsValidDisposal() const
{
	if (!WasGrabbed) return false;
	if (ReleaseTime > 5.0f) return false;
	return true;
}

