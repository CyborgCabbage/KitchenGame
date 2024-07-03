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
	GrabberOwner = EGrabberOwner::IMP;
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

bool UGrabber::CanPickup(UGrabbable* grabbable)
{
	//Duh
	if (!grabbable->CanGrab) return false;
	//Both players and imps can pickup locked stuff
	if(grabbable->IsLocked) return true;
	//If an imp owns it, no one can take it (not even other imps)
	if (grabbable->IsGrabbed) {
		if (grabbable->Grabber->GrabberOwner == EGrabberOwner::IMP) {
			return false;
		}
	}
	return true;
}

void UGrabber::TryPickup(UGrabbable* grabbable)
{
	if (!IsValid(grabbable)) return;
	if (!CanPickup(grabbable)) return;
	if (IsValid(Grabbed)) return;
	UGrabber* stolenFrom = grabbable->IsGrabbed ? grabbable->Grabber : nullptr;
	grabbable->UnlockIfLocked();
	if (grabbable->IsGrabbed && IsValid(grabbable->Grabber)) {
		grabbable->Grabber->TryDrop();
	}
	Grabbed = grabbable;
	Grabbed->IsGrabbed = true;
	Grabbed->Grabber = this;
	if(!FinishPickup()) {
		Grabbed->IsGrabbed = false;
		Grabbed->Grabber = nullptr;
		Grabbed = nullptr;
	}
	if (stolenFrom) {
		stolenFrom->OnStolenDelegate.ExecuteIfBound(grabbable);
	}
}

bool UGrabber::FinishPickup() {
	//Physics grab
	if (!IsValid(PhysicsHandle)) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to pick up object, could not find PhysicsHandle"));
		return false;
	}
	Grabbed->SetEnablePhysics(true, true);
	FName BoneName = NAME_None;
	if(auto* SkelMesh = Cast<USkeletalMeshComponent>(Grabbed->GrabTarget)) {
		BoneName = SkelMesh->GetSocketBoneName("Grab");
	}
	//TODO: this is scuffed, remove the grab lockpoint if we are not using it
	FName Socket = Grabbed->GrabTarget->DoesSocketExist("Bottom") ? "Bottom" : "Grab";
	PhysicsHandle->GrabComponentAtLocationWithRotation(
		Grabbed->GrabTarget,
		BoneName,
		Grabbed->GrabTarget->GetSocketLocation(Socket),
		Grabbed->GrabTarget->GetSocketRotation(Socket)
	);
	return IsValid(PhysicsHandle->GetGrabbedComponent());
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