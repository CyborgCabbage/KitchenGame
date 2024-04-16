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
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->GetComponentByClass<UPhysicsHandleComponent>();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGrabber::SetTarget(FVector Location, FRotator Rotation)
{
	PhysicsHandle->SetTargetLocationAndRotation(Location, Rotation);
}

void UGrabber::TryPickup(UGrabbable* grabbable)
{
	if (!grabbable->CanGrab) return;
	if (IsValid(Grabbed)) return;
	Grabbed = grabbable;
	Grabbed->IsGrabbed = true;
	Grabbed->Grabber = this->GetOwner();//TODO
	Grabbed->UnlockIfLocked();
	if(!FinishPickup()) {
		Grabbed->IsGrabbed = false;
		Grabbed->Grabber = nullptr;
		Grabbed = nullptr;
	}
}

bool UGrabber::FinishPickup() {
	//Physics grab
	if (!IsValid(PhysicsHandle)) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to pick up object, could not find PhysicsHandle"));
		return false;
	}
	Grabbed->SetEnablePhysics(true, true);
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		Grabbed->GrabTarget,
		NAME_None,
		Grabbed->GrabTarget->GetComponentLocation(),
		Grabbed->GrabTarget->GetComponentRotation()
	);
	return true;
}

void UGrabber::TryDrop()
{
	if (!IsValid(Grabbed)) return;
	Grabbed->IsGrabbed = false;
	Grabbed->Grabber = nullptr;
	FinishDrop();
	Grabbed = nullptr;
}

UGrabbable* UGrabber::GetGrabbed()
{
	return Grabbed;
}

bool UGrabber::IsGrabbing()
{
	return IsValid(Grabbed);
}

void UGrabber::FinishDrop() {
	if (IsValid(PhysicsHandle)) {
		PhysicsHandle->ReleaseComponent();
	}
}