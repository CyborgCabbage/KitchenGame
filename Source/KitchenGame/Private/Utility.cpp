// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/UserInterfaceSettings.h"

UUtility::UUtility()
{
}

UUtility::~UUtility()
{
}

void UUtility::MoveToTransform(USceneComponent* ToMove, USceneComponent* Origin, USceneComponent* Destination, bool NoRotation) {
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

FVector2D UUtility::ProjectWorldToScreen(APlayerController const* Player, FVector WorldPosition)
{
	ULocalPlayer* const LP = Player->GetLocalPlayer();
	if (LP && LP->ViewportClient)
	{
		// get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			FVector2D ScreenPosition;
			bool bResult = FSceneView::ProjectWorldToScreen(WorldPosition, ProjectionData.GetConstrainedViewRect(), ViewProjectionMatrix, ScreenPosition);
			//Relative Viewport
			ScreenPosition -= FVector2D(ProjectionData.GetConstrainedViewRect().Min);
			bResult = bResult && Player->PostProcessWorldToScreen(WorldPosition, ScreenPosition, true);
			//Adjust to viewport scale
			FVector2D ViewportSize;
			LP->ViewportClient->GetViewportSize(ViewportSize);
			float UserResolutionScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
			ScreenPosition /= UserResolutionScale;
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, ScreenPosition.ToString());
			ScreenPosition.X = FMath::Clamp(ScreenPosition.X, 0, ViewportSize.X / UserResolutionScale);
			ScreenPosition.Y = FMath::Clamp(ScreenPosition.Y, 0, ViewportSize.Y / UserResolutionScale);
			return ScreenPosition;
		}
	}
	return FVector2D::ZeroVector;
}


