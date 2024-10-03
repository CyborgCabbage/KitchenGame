// Fill out your copyright notice in the Description page of Project Settings.


#include "LockPoint.h"
#include "Grabbable.h"
#include "LockPointTrigger.h"
#include "Utility.h"


// Sets default values for this component's properties
ULockPoint::ULockPoint() : TriggerRadius(0.4), TriggerHeight(0.3)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockPoint::BeginPlay()
{
	Super::BeginPlay();

	SetEnabled(true);
	
}

void ULockPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed) {
		if (IsValid(LockedItem)) {
			LockedItem->SetEnablePhysics(true, true);
			LockedItem->LockPoint = nullptr;
			LockedItem->IsLocked = false;
			LockedItem = nullptr;
		}
	}
}


// Called every frame
void ULockPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULockPoint::SetEnabled(bool Enabled) {
	if(Enabled == ActorEnabled) return;
	ActorEnabled = Enabled;
	if(ActorEnabled) {
		FTransform transform = GetComponentTransform();
		transform.SetScale3D({1,1,1});
		FActorSpawnParameters param{};
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		param.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
		ALockPointTrigger* triggerActor = GetWorld()->SpawnActor<ALockPointTrigger>(TriggerClass, transform, param);
		if(triggerActor == nullptr) return;
		triggerActor->SetActorScale3D({TriggerRadius, TriggerRadius, TriggerHeight});
		triggerActor->ParentLockPoint = this;
		triggerActor->AttachToComponent(this, { EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true});
		CurrentActor = triggerActor;
	}else {
		CurrentActor->Destroy();
		CurrentActor = nullptr;
	}
}

void ULockPoint::LockItem(UGrabbable* Grabbed) {

	//UUtility::MoveToTransform(Grabbed->GrabTarget, Grabbed->BottomPoint, this, false);
	LockedItem = Grabbed;
	InUse = true;
	LockedItem->GrabTarget->SetWorldTransform(GetLockItemTransform(Grabbed));
	LockedItem->SetEnablePhysics(false, true);
	LockedItem->IsLocked = true;
	LockedItem->LockPoint = this;
	LockedItem->GetOwner()->AttachToActor(GetOwner(), {EAttachmentRule::KeepWorld, true});
	/*LockedItem->GetOwner()->SetActorRelativeLocation(FVector::ZeroVector);
	LockedItem->GetOwner()->SetActorRelativeRotation(FQuat::Identity);
	FTransform SocketTransform = LockedItem->GrabTarget->GetSocketTransform("Bottom").GetRelativeTransform(GetComponentTransform());
	SocketTransform.SetScale3D(FVector::OneVector);
	SocketTransform = SocketTransform.Inverse();
	LockedItem->GetOwner()->SetActorRelativeLocation(SocketTransform.GetLocation());
	LockedItem->GetOwner()->SetActorRelativeRotation(SocketTransform.GetRotation());*/
	SetEnabled(false);
}

FTransform ULockPoint::GetLockItemTransform(UGrabbable* Grabbed)
{
	return UUtility::MoveToTransform2(Grabbed->GrabTarget, Grabbed->GrabTarget->GetSocketTransform("Bottom"), GetComponentTransform(), false);
}

void ULockPoint::UnlockItem() {
	if(!InUse) return;
	InUse = false;
	if (IsValid(LockedItem)) {
		LockedItem->SetEnablePhysics(true, true);
		LockedItem->LockPoint = nullptr;
		LockedItem->IsLocked = false;
		LockedItem->GetOwner()->DetachFromActor({ EDetachmentRule::KeepWorld, true });
		LockedItem = nullptr;
	}
	SetEnabled(true);
}

bool ULockPoint::CanLock(UGrabbable* Grabbable) {
	//TODO: Slug bodge
	if (USkeletalMeshComponent* SkelGrabTarget = Cast<USkeletalMeshComponent>(Grabbable->GrabTarget)) {
		if (AIngredient* Ingredient = Cast<AIngredient>(GetOwner())) {
			if (Ingredient->SauceType != ESauceType::None) {
				return false;
			}
		}
	}
	AActor* actor = Grabbable->GetOwner();
	for(const UClass* allowedClass : AllowedClasses) {
		if(actor->IsA(allowedClass)) {
			return true;
		}
	}
	return false;
}

ALockPointTrigger* ULockPoint::GetTriggerActor(bool& Enabled) {
	Enabled = (CurrentActor != nullptr);
	return CurrentActor;
}
