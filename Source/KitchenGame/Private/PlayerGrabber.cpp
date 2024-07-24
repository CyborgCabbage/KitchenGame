// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGrabber.h"
#include "Grabbable.h"
#include "Utility.h"
#include "LockPoint.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
UPlayerGrabber::UPlayerGrabber() : 
	AttachedToHand(false),
	IsSplat(false),
	DropDistance(100), 
	GrabDistance(120), 
	TraceDistance(150), 
	TraceRadius(5)
{
	GrabberOwner = EGrabberOwner::PLAYER;
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
	if (Grabbed->InHand) {
		//Hand grab
		if (!IsValid(Hand)) {
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to pick up object, could not find Hand"));
			return false;
		}
		Grabbed->GrabTarget->SetWorldTransform(UUtility::MoveToTransform2(Grabbed->GrabTarget, Grabbed->GrabTarget->GetSocketTransform("Grab"), Hand->GetComponentTransform(), false));
		Grabbed->SetEnablePhysics(false, false);
		Grabbed->GetOwner()->AttachToComponent(Hand, {EAttachmentRule::KeepWorld, true});
		AttachedToHand = true;
		return true;
	}
	else {
		return UGrabber::FinishPickup();
	}
}

void UPlayerGrabber::FinishDrop() {
	if (CurrentLockPoint) {
		Grabbed->OnLockPointEndOverlapDelegate.ExecuteIfBound(CurrentLockPoint);
		IsSplat = false;
		CurrentLockPoint = nullptr;
	}
	if (AttachedToHand) {
		Grabbed->SetEnablePhysics(true, true);
		Grabbed->GetOwner()->DetachFromActor({ EDetachmentRule::KeepWorld, true });
		AttachedToHand = false;
		FTransform OriginTransform = UUtility::MoveToTransform2(
			Grabbed->GrabTarget, 
			Grabbed->GrabTarget->GetSocketTransform("Bottom", RTS_World),
			View->GetComponentTransform(),
			true
		);
		FVector Begin = OriginTransform.GetLocation();
		FVector Direction = View->GetForwardVector();
		TArray<FHitResult> OutHits;
		FComponentQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.AddIgnoredActor(Grabbed->GetOwner());
		TArray<AActor*> Attached;
		Grabbed->GetOwner()->GetAttachedActors(Attached, true, true);
		QueryParams.AddIgnoredActors(Attached);
		Grabbed->GetWorld()->ComponentSweepMultiByChannel(OutHits, Grabbed->GrabTarget, Begin, Begin + Direction * TraceDistance, OriginTransform.GetRotation(), ECollisionChannel::ECC_Camera, QueryParams);
		FVector Location = Begin + Direction * TraceDistance;
		for (const FHitResult& Hit : OutHits) {
			if (!Hit.bBlockingHit) continue;
			Location = Hit.Location;
			break;
		}
		Grabbed->GrabTarget->SetWorldLocationAndRotation(Location, OriginTransform.GetRotation());
		Grabbed->GrabTarget->AddImpulse(GetOwner()->GetVelocity(), NAME_None, true);
	}
	else {
		UGrabber::FinishDrop();
	}
}

ALockPointTrigger* UPlayerGrabber::TraceLockPoint() {
	check(IsValid(Grabbed))
	FVector Begin = View->GetComponentTransform().GetLocation();
	FVector Direction = View->GetComponentTransform().GetRotation().GetForwardVector();
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	//Ignore player and grabbed
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(Grabbed->GetOwner());
	TArray<AActor*> Attached;
	Grabbed->GetOwner()->GetAttachedActors(Attached, true, true);
	QueryParams.AddIgnoredActors(Attached);
	GetWorld()->SweepMultiByChannel(OutHits, Begin, Begin + Direction * TraceDistance, FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeSphere(TraceRadius), QueryParams);
	for (const FHitResult& Hit : OutHits) {
		if (auto* LockPointTrigger = Cast<ALockPointTrigger>(Hit.GetActor())) {
			if (LockPointTrigger->ParentLockPoint->CanLock(Grabbed)) {
				return LockPointTrigger;
			}
		}
	}
	return nullptr;
}

void UPlayerGrabber::UpdateCurrentLockPoint()
{
	if(!IsValid(Grabbed)) {
		//TryDropToLockPoint should set currentlockpoint to null if the item is dropped,
		//so we don't have do anything here
		return;
	}
	ALockPointTrigger* LPT = TraceLockPoint();
	ULockPoint* NextLockPoint = LPT ? LPT->ParentLockPoint : nullptr;
	if (NextLockPoint != CurrentLockPoint) {
		if(IsValid(CurrentLockPoint)) {
			Grabbed->OnLockPointEndOverlapDelegate.ExecuteIfBound(CurrentLockPoint);
			IsSplat = false;
		}
		if (IsValid(NextLockPoint) && Grabbed->OnLockPointBeginOverlapDelegate.IsBound()) {
			IsSplat = Grabbed->OnLockPointBeginOverlapDelegate.Execute(NextLockPoint);
		}
		CurrentLockPoint = NextLockPoint;
	}
	return;
}

void UPlayerGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UGrabber::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Begin = View->GetComponentTransform().GetLocation();
	FVector Direction = View->GetComponentTransform().GetRotation().GetForwardVector();
	FTransform Target{ FRotator{ 0, View->GetComponentRotation().Yaw, 0 }, Begin + Direction * GrabDistance};
	UpdateCurrentLockPoint();
	//Update visual
	if(CurrentLockPoint) {
		bool TriggerEnabled = false;
		ALockPointTrigger* LockPointTrigger = CurrentLockPoint->GetTriggerActor(TriggerEnabled);
		LockPointTrigger->SetEnableVisual(IsSplat);
		//Target = LockPointTrigger->ParentLockPoint->GetLockItemTransform(Grabbed);
		Target = UUtility::MoveToTransform2(Grabbed->GrabTarget, Grabbed->GrabTarget->GetSocketTransform("Bottom"), LockPointTrigger->ParentLockPoint->GetComponentTransform(), false);
		float Sinus = (sinf(UGameplayStatics::GetRealTimeSeconds(this) * 2 * PI / 1.5f) + 2.0f) * 1.5f;
		Target.AddToTranslation(LockPointTrigger->GetActorUpVector().GetUnsafeNormal() * Sinus);
		Target.SetTranslation(FMath::Lerp(Target.GetTranslation(), Begin + Direction * GrabDistance, 0.2f));
	}
	//Move grabbed to location
	if (IsValid(Grabbed) && !AttachedToHand) {
		SetTarget(Target.GetLocation(), Target.Rotator());
	}
}

void UPlayerGrabber::TryDropToLockPoint()
{
	if (!IsValid(Grabbed)) return;
	ULockPoint* CurrentLockPointTemp = CurrentLockPoint;
	UGrabbable* Grabbable = Grabbed;
	TryDrop();
	if (CurrentLockPointTemp) {
		CurrentLockPointTemp->LockItem(Grabbable);
	}
}

bool UPlayerGrabber::IsOverLockPoint() {
	return IsValid(CurrentLockPoint) && IsValid(Grabbed);
}
