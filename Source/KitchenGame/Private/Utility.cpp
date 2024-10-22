// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/UserInterfaceSettings.h"
#include "GameFramework/Character.h"
#include "Grabbable.h"

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
	if (!IsValid(Actor)) {
		UE_LOG(LogTemp, Warning, TEXT("Parent actor in GetActorAndAttached was invalid."));
		return;
	}
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

int UUtility::SumScoreParts(TArray<FScorePart> Parts)
{
	int Total = 0;
	int PercentageTotal = 0;
	for (const FScorePart& Part : Parts) {
		if (Part.IsPercentage)
		{
			PercentageTotal += Part.Value;
		} 
		else
		{
			Total += Part.Value;
		}
	}
	return Total + Total * (PercentageTotal / 100.0f);
}

int UUtility::SumRecipeScore(TArray<FRecipeScore> Parts)
{
	int Total = 0;
	for (const FRecipeScore& Part : Parts) {
		Total += SumScoreParts(Part.Parts);
	}
	return Total;
}

struct ConsolidatedScoreData {
	FText Reason;
	int Sum;
	int Count;
};

TArray<FScorePart> UUtility::ConsolidateRecipeScore(const TArray<FScorePart>& Parts)
{
	TArray<FScorePart> ConsolidatedParts;
	TMap<FName, ConsolidatedScoreData> ScoreMap;
	for (const FScorePart& Part : Parts) {
		if (Part.ConsolidationId.IsNone()) {
			ConsolidatedParts.Add(Part);
		}
		else {
			if (auto* Temp = ScoreMap.Find(Part.ConsolidationId)) {
				Temp->Sum += Part.Value;
				Temp->Count++;
			}
			else {
				ScoreMap.Add({ Part.ConsolidationId, ConsolidatedScoreData{Part.Reason, Part.Value, 1} });
			}
		}
	}
	for (const auto& Pair : ScoreMap) {
		if(Pair.Value.Count > 1) {
			ConsolidatedParts.Add(FScorePart{ Pair.Value.Sum, FText::Format(FText::FromString("{0} x{1}"), Pair.Value.Reason, Pair.Value.Count) });
		} else {
			ConsolidatedParts.Add(FScorePart{ Pair.Value.Sum, Pair.Value.Reason });
		}
	}
	return ConsolidatedParts;
}
void UUtility::LaunchActor(AActor* Actor, FVector Velocity, bool bAddToCurrent) {
	bool LaunchedSomething = false;
	TArray<UPrimitiveComponent*> PrimMeshes;
	Actor->GetComponents<UPrimitiveComponent>(PrimMeshes);
	for(UPrimitiveComponent* PrimMesh : PrimMeshes) {
		if (PrimMesh->IsAnySimulatingPhysics()) {
			PrimMesh->WakeAllRigidBodies();
			PrimMesh->SetAllPhysicsLinearVelocity(Velocity, bAddToCurrent);
			LaunchedSomething = true;
		}
	}
	if (!LaunchedSomething) {
		if(ACharacter* Character = Cast<ACharacter>(Actor)) {
			Character->LaunchCharacter(Velocity, !bAddToCurrent, !bAddToCurrent);
		}
	}
}

TMap<ESnapDirection, FSnapMarkerInfo> UUtility::CalculateSnapMarkers(APlayerController const* Player, const TSet<UActorComponent*>& ComponentsInRange, const TArray<AActor*>& IgnoreActors, float ScreenBorder) {
	TSet<AActor*> Actors;
	for (const auto& Component : ComponentsInRange) {
		if(IsValid(Component) && IsValid(Component->GetOwner())) {
			Actors.Add(Component->GetOwner());
		}
	}
	for (AActor* Ignored : IgnoreActors) {
		Actors.Remove(Ignored);
	}
	TMap<ESnapDirection, FSnapMarkerInfo> Chosen;
	ULocalPlayer* const LP = Player->GetLocalPlayer();
	if (!LP || !LP->ViewportClient) {
		return {};
	}
	//Viewport Size
	FVector2D ViewportSize;
	LP->ViewportClient->GetViewportSize(ViewportSize);
	FVector2D ViewportCentre = ViewportSize / 2;
	//Get the projection data
	FSceneViewProjectionData ProjectionData;
	if (!LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData)) {
		return {};
	}
	FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
	for (AActor* Actor : Actors) {
		FVector Pos = FVector::ZeroVector;
		UGrabbable* Grabbable = Actor->GetComponentByClass<UGrabbable>();
		if (Grabbable) {
			Pos = Grabbable->GetCenterOfMass();
		}
		if (Pos == FVector::ZeroVector) {
			continue;
		}
		FVector2D ScreenPosition = FVector2D::ZeroVector;
		FSceneView::ProjectWorldToScreen(Pos, ProjectionData.GetConstrainedViewRect(), ViewProjectionMatrix, ScreenPosition);
		//Relative Viewport
		ScreenPosition -= FVector2D(ProjectionData.GetConstrainedViewRect().Min);
		//Custom Post Process
		Player->PostProcessWorldToScreen(Pos, ScreenPosition, true);
		//Out of Circle
		if (!FBox2D(FVector2D::ZeroVector, ViewportSize).ExpandBy(-ViewportCentre.GetMin() * ScreenBorder).IsInside(ScreenPosition)) {
			continue;
		}
		//Get longest axis
		FVector2D RelativePosition = ScreenPosition - ViewportCentre;
		ESnapDirection ShortestAxis;
		float ShortestAxisLength;
		if (FMath::Abs(RelativePosition.X) > FMath::Abs(RelativePosition.Y)) {
			ShortestAxisLength = FMath::Abs(RelativePosition.X);
			if (RelativePosition.X > 0) {
				ShortestAxis = ESnapDirection::Right;
			} else {
				ShortestAxis = ESnapDirection::Left;
			}
		} else {
			ShortestAxisLength = FMath::Abs(RelativePosition.Y);
			if (RelativePosition.Y > 0) {
				ShortestAxis = ESnapDirection::Down;
			} else {
				ShortestAxis = ESnapDirection::Up;
			}
		}
		auto& Entry = Chosen.FindOrAdd(ShortestAxis);
		float DistanceToCentre = FVector2D::Distance(ViewportCentre, ScreenPosition);
		if (DistanceToCentre < Entry.DistanceAlongAxis) {
			//Adjust to viewport scale
			float UserResolutionScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
			ScreenPosition /= UserResolutionScale;
			Entry.PositionOnScreen = ScreenPosition;
			Entry.DistanceAlongAxis = DistanceToCentre;
			Entry.WorldPosition = Pos;
			Entry.Grabbable = Grabbable;
		}
	}
	return Chosen;
}

FVector2D UUtility::GetSnapMarkerPositionOnScreen(APlayerController const* Player, const FSnapMarkerInfo& SnapMarkerInfo) {
	ULocalPlayer* const LP = Player->GetLocalPlayer();
	if (!LP || !LP->ViewportClient) {
		return {};
	}
	//Viewport Size
	FVector2D ViewportSize;
	LP->ViewportClient->GetViewportSize(ViewportSize);
	FVector2D ViewportCentre = ViewportSize / 2;
	//Get the projection data
	FSceneViewProjectionData ProjectionData;
	if (!LP->GetProjectionData(LP->ViewportClient->Viewport, /*out*/ ProjectionData)) {
		return {};
	}
	FMatrix const ViewProjectionMatrix = ProjectionData.ComputeViewProjectionMatrix();
	FVector2D ScreenPosition = FVector2D::ZeroVector;
	FSceneView::ProjectWorldToScreen(SnapMarkerInfo.WorldPosition, ProjectionData.GetConstrainedViewRect(), ViewProjectionMatrix, ScreenPosition);
	//Relative Viewport
	ScreenPosition -= FVector2D(ProjectionData.GetConstrainedViewRect().Min);
	//Custom Post Process
	Player->PostProcessWorldToScreen(SnapMarkerInfo.WorldPosition, ScreenPosition, true);
	//Adjust to viewport scale
	float UserResolutionScale = GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(FIntPoint(ViewportSize.X, ViewportSize.Y));
	ScreenPosition /= UserResolutionScale;
	return ScreenPosition;
}

