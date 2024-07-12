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

FTransform UUtility::MoveToTransform2(USceneComponent* ToMove, FTransform Origin, FTransform Destination, bool NoRotation) {
	//Get Transforms and normalize scale
	FTransform toMoveTransform = ToMove->GetComponentTransform();
	toMoveTransform.SetScale3D({ 1,1,1 });
	Origin.SetScale3D({ 1,1,1 });
	Destination.SetScale3D({ 1,1,1 });
	if (NoRotation) {
		FQuat rotation = Destination.GetRotation();
		rotation = FQuat::MakeFromEuler({ 0, 0, rotation.Euler().Z });
		Destination.SetRotation(rotation);
	}
	FTransform temp = UKismetMathLibrary::ComposeTransforms(Origin, toMoveTransform.Inverse());
	FTransform toMove = UKismetMathLibrary::ComposeTransforms(temp.Inverse(), Destination);
	toMove.SetScale3D(ToMove->GetComponentScale());
	return toMove;
}
FVector2D UUtility::ProjectWorldToScreen(APlayerController const* Player, FVector WorldPosition, float& ArrowAngle)
{
	ArrowAngle = 0;
	ULocalPlayer* const LP = Player->GetLocalPlayer();
	if (LP && LP->ViewportClient)
	{
		//Viewport Size
		FVector2D ViewportSize;
		LP->ViewportClient->GetViewportSize(ViewportSize);
		FVector2D ViewportCentre = ViewportSize / 2;
		float Radius = ViewportCentre.GetMin() * 0.8f;
		FVector2D ScreenPosition = FVector2D::ZeroVector;
		//Get the projection data
		FSceneViewProjectionData ProjectionData;
		if (LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData))
		{
			FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
			FSceneView::ProjectWorldToScreen(WorldPosition, ProjectionData.GetConstrainedViewRect(), ViewProjectionMatrix, ScreenPosition);
			//Relative Viewport
			ScreenPosition -= FVector2D(ProjectionData.GetConstrainedViewRect().Min);
			//Custom Post Process
			Player->PostProcessWorldToScreen(WorldPosition, ScreenPosition, true);
			//Out of Circle
			if (FVector2D::Distance(ViewportCentre, ScreenPosition) > Radius) {
				FMatrix ViewTransform = FTranslationMatrix(-ProjectionData.ViewOrigin) * ProjectionData.ViewRotationMatrix;
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Result.ToString());
				ScreenPosition = FVector2D(ViewTransform.TransformFVector4(FVector4(WorldPosition, 1.f)));
				ScreenPosition.Y *= -1;
				ScreenPosition.Normalize();
				ArrowAngle = FMath::RadiansToDegrees(FMath::Atan2(ScreenPosition.Y, ScreenPosition.X)) - 90.0f;
				ScreenPosition = ScreenPosition * Radius + ViewportCentre;
			}
		}
		//Adjust to viewport scale
		float UserResolutionScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
		ScreenPosition /= UserResolutionScale;
		return ScreenPosition;
	}
	
	return FVector2D::ZeroVector;
}

FString UUtility::WithCapital(const FString& SInput)
{
	FString SOutput = SInput;
	if (SOutput.Len() > 0) {
		auto& Character = SOutput.GetCharArray()[0];
		Character = toupper(Character);
	}
	return SOutput;
}

void UUtility::DestroyActorAndAttached(AActor* Actor)
{
	TArray<AActor*> OutActors;
	GetActorAndAttached(OutActors, Actor);
	for (AActor* OutActor : OutActors) {
		OutActor->Destroy();
	}
}

void UUtility::GetActorAndAttached(TArray<AActor*>& OutActors, AActor* Actor) {
	OutActors.Add(Actor);
	Actor->GetAttachedActors(OutActors, false, true);
}

ESecondaryCookPhase UUtility::GetRandomCookPhase()
{
	return (ESecondaryCookPhase)FMath::RandRange(1, 3);
}

ESauceType UUtility::GetRandomSauceType()
{
	return (ESauceType)FMath::RandRange(1, 3);
}


