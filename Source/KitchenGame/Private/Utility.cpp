// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"
#include "Kismet/KismetMathLibrary.h"

Utility::Utility()
{
}

Utility::~Utility()
{
}

void Utility::MoveToTransform(USceneComponent* ToMove, USceneComponent* Origin, USceneComponent* Destination, bool NoRotation) {
	//Get Transforms and normalize scale
	FTransform toMoveTransform = ToMove->GetComponentTransform();
	toMoveTransform.SetScale3D({1,1,1});
	FTransform originTransform = Origin->GetComponentTransform();
	originTransform.SetScale3D({1,1,1});
	FTransform destinationTransform = Destination->GetComponentTransform();
	destinationTransform.SetScale3D({1,1,1});
	if (NoRotation) {
		FQuat rotation = destinationTransform.GetRotation();
		rotation = FQuat::MakeFromEuler({0, 0, rotation.Euler().Z});
		destinationTransform.SetRotation(rotation);
	}
	FTransform temp = UKismetMathLibrary::ComposeTransforms(originTransform, toMoveTransform.Inverse());
	FTransform toMove = UKismetMathLibrary::ComposeTransforms(temp.Inverse(), destinationTransform);
	toMove.SetScale3D(ToMove->GetComponentScale());
	ToMove->SetWorldTransform(toMove, false, nullptr, ETeleportType::TeleportPhysics);
}


