// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGrabber.h"
#include "Grabbable.h"
#include "Utility.h"
#include "LockPoint.h"

UPlayerGrabber::UPlayerGrabber() : AttachedToHand(false), GrabMax(250), GrabMin(100), GrabCurrent(0) {
}

void UPlayerGrabber::BeginPlay()
{
	UGrabber::BeginPlay();
	auto ViewComponents = GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), "View");
	View = Cast<USceneComponent>(ViewComponents[0]);
	auto HandComponents = GetOwner()->GetComponentsByTag(USceneComponent::StaticClass(), "Hand");
	Hand = Cast<USceneComponent>(HandComponents[0]);
}

bool UPlayerGrabber::FinishPickup() {
	GrabCurrent = GrabMin;
	if (Grabbed->InHand) {
		//Hand grab
		if (!IsValid(Hand)) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to pick up object, could not find Hand"));
			return false;
		}
		Grabbed->SetEnablePhysics(false, false);
		Grabbed->GetOwner()->AttachToComponent(Hand, {EAttachmentRule::KeepWorld, true});
		UUtility::MoveToTransform(Grabbed->GrabTarget, Grabbed->GrabPoint, Hand, false);
		AttachedToHand = true;
		return true;
	}
	else {
		return UGrabber::FinishPickup();
	}
}

void UPlayerGrabber::FinishDrop() {
	if (AttachedToHand) {
		Grabbed->SetEnablePhysics(true, true);
		Grabbed->GetOwner()->DetachFromActor(FDetachmentTransformRules{ EDetachmentRule::KeepWorld, true });
		AttachedToHand = false;
	}
	else {
		UGrabber::FinishDrop();
	}
}

ALockPointTrigger* UPlayerGrabber::TraceLockPoint() {
	if (!IsValid(Grabbed)) return nullptr;
	FVector Begin = View->GetComponentTransform().GetLocation();
	FVector Direction = View->GetComponentTransform().GetRotation().GetForwardVector();
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceMultiByChannel(OutHits, Begin, Begin + Direction * GrabMax, ECollisionChannel::ECC_Camera, QueryParams);
	for (const FHitResult& Hit : OutHits) {
		if (auto* LockPointTrigger = Cast<ALockPointTrigger>(Hit.GetActor())) {
			if (LockPointTrigger->ParentLockPoint->CanLock(Grabbed)) {
				return LockPointTrigger;
			}
		}
	}
	return nullptr;
}

ALockPointTrigger* UPlayerGrabber::OverlapLockPoint() {
	if (!IsValid(Grabbed)) return nullptr;
	TArray<AActor*> Overlapping;
	Grabbed->GrabTarget->GetOverlappingActors(Overlapping, ALockPointTrigger::StaticClass());
	for (AActor* Actor : Overlapping) {
		auto* LockPointTrigger = Cast<ALockPointTrigger>(Actor);
		if (LockPointTrigger->ParentLockPoint->CanLock(Grabbed)) {
			return LockPointTrigger;
		}
	}
	return nullptr;
}

void UPlayerGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UGrabber::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Move grabbed to location
	if (IsValid(Grabbed) && !AttachedToHand) {
		FVector Begin = View->GetComponentTransform().GetLocation();
		FVector Direction = View->GetComponentTransform().GetRotation().GetForwardVector();
		FVector Offset = Grabbed->GrabTarget->GetComponentLocation() - Grabbed->GrabPoint->GetComponentLocation();
		SetTarget(Begin + Direction * GrabCurrent + Offset, FRotator{ 0, View->GetComponentRotation().Yaw, 0});
	}
	//Update visual
	if (auto* LockPointTrigger = AttachedToHand ? TraceLockPoint() : OverlapLockPoint()) {
		LockPointTrigger->SetEnableVisual();
	}
}

void UPlayerGrabber::TryDropToLockPoint()
{
	if (!IsValid(Grabbed)) return;
	ALockPointTrigger* LockPointTrigger = AttachedToHand ? TraceLockPoint() : OverlapLockPoint();
	UGrabbable* Grabbable = Grabbed;
	TryDrop();
	if (LockPointTrigger) {
		LockPointTrigger->ParentLockPoint->LockItem(Grabbable);
	}
}

void UPlayerGrabber::AdjustGrabDistance(float Change)
{
	GrabCurrent = FMath::Clamp(GrabCurrent + Change, GrabMin, GrabMax);
}
