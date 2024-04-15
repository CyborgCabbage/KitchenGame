// Fill out your copyright notice in the Description page of Project Settings.


#include "LockPoint.h"
#include "Grabbable.h"
#include "LockPointTrigger.h"
#include "Utility.h"


// Sets default values for this component's properties
ULockPoint::ULockPoint()
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

	if(AlwaysEnabled) {
		SetEnabled(true);
	}
	
}


// Called every frame
void ULockPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(AlwaysEnabled) return;
	if(InUse) return;
	if(auto* grabbable = GetOwner()->GetComponentByClass<UGrabbable>()) {
		if(grabbable->IsLocked) {
			auto lp = grabbable->LockPoint;
			if(!lp->AlwaysEnabled || lp->AllowStack) {
				SetEnabled(true);
				return;
			}
		}
	}
	SetEnabled(false);
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
		triggerActor->ParentLockPoint = this;
		triggerActor->AttachToComponent(this, { EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true});
		CurrentActor = triggerActor;
	}else {
		CurrentActor->Destroy();
		CurrentActor = nullptr;
	}
}

void ULockPoint::LockItem(UGrabbable* Grabbed) {
	Utility::MoveToTransform(Grabbed->GrabTarget, Grabbed->BottomPoint, this, false);
	Grabbed->SetEnablePhysics(false, true);
	InUse = true;
	LockedItem = Grabbed;
	Grabbed->IsLocked = true;
	Grabbed->LockPoint = this;
	Grabbed->GetOwner()->AttachToActor(GetOwner(), { EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true});
	SetEnabled(false);
}

void ULockPoint::UnlockItem() {
	if(!InUse) return;
	InUse = false;
	if(!IsValid(LockedItem)) return;
	LockedItem->IsLocked = false;
	LockedItem->SetEnablePhysics(true, true);
	SetEnabled(true);
}

bool ULockPoint::CanLock(UGrabbable* Grabbable) {
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