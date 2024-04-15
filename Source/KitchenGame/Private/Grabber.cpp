// Fill out your copyright notice in the Description page of Project Settings.
#include "Grabber.h"
#include "Grabbable.h"
#include "Utility.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Grabbed = nullptr;
	Hand = nullptr;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->GetComponentByClass<UPhysicsHandleComponent>();

	// ...

}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::SetHand(USceneComponent* sceneComponent)
{
	Hand = sceneComponent;
}

void UGrabber::SetTargetTransform(FTransform transform)
{
	TargetLocation = transform;
	PhysicsHandle->SetTargetLocationAndRotation(transform.GetLocation(), transform.GetRotation().Rotator());
}

void UGrabber::TryPickup(UGrabbable* grabbable)
{
	if (!grabbable->CanGrab) return;
	if (IsValid(Grabbed)) return;
	Grabbed = grabbable;
	Grabbed->IsGrabbed = true;
	Grabbed->Grabber = this->GetOwner();//TODO
	Grabbed->UnlockIfLocked();
	if (IsValid(Hand) && Grabbed->InHand) {
		//Hand grab
		Grabbed->SetEnablePhysics(false, false);
		Grabbed->GetOwner()->AttachToComponent(Hand, {EAttachmentRule::KeepWorld, true});
		Utility::MoveToTransform(Grabbed->GrabTarget, Grabbed->GrabPoint, Hand, false);
		AttachedToHand = true;
	}
	else if(IsValid(PhysicsHandle)){
		//Physics grab
		Grabbed->SetEnablePhysics(true, true);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			Grabbed->GrabTarget,
			NAME_None, 
			Grabbed->GrabTarget->GetComponentLocation(), 
			Grabbed->GrabTarget->GetComponentRotation()
		);
		TargetLocation = Grabbed->GrabTarget->GetComponentTransform();
	}
	else {
		if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to pick up object, could not find PhysicsHandle"));
		Grabbed->IsGrabbed = false;
		Grabbed->Grabber = nullptr;
		Grabbed = nullptr;
	}
}

void UGrabber::TryDrop()
{
	if (!IsValid(Grabbed)) return;
	UGrabbable* grabbable = Grabbed;
	Grabbed = nullptr;
	grabbable->IsGrabbed = false;
	grabbable->Grabber = nullptr;
	if (AttachedToHand) {
		grabbable->SetEnablePhysics(true, true);
		grabbable->GetOwner()->DetachFromActor(FDetachmentTransformRules{ EDetachmentRule::KeepWorld, true});
	}
	else {
		PhysicsHandle->ReleaseComponent();
	}
}

