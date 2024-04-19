// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Grabber.h"
#include "Grabbable.h"
#include "AIngredient.h"
void UTutorialStep::Activate()
{
	TutorialManager->AddTutorialStep(this);
}

bool UTutorialStep::IsPassed()
{
	if (Remember && Passed) {
		return true;
	}
	Passed = false;
	Check.Broadcast();
	return Passed;
}

// Sets default values
ATutorialManager::ATutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsActive = false;
	CurrentStep = 0;
	CurrentInstruction.FromString("");
	Highlighted = nullptr;
}

void ATutorialManager::SetInstruction(const FText& Text)
{
	if (!CurrentInstruction.EqualTo(Text)) {
		OnInstructionChange(Text);
		CurrentInstruction = Text;
	}
}

// Called when the game starts or when spawned
void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATutorialManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Clear Highlights
	Highlighted = nullptr;
	//
	if (!IsActive) {
		SetInstruction({});
		OnHighlightChange(Highlighted);
		return;
	}
	//Select current step
	for (CurrentStep = 0; CurrentStep < Steps.Num(); CurrentStep++) {
		if (Steps[CurrentStep]->Essential && !Steps[CurrentStep]->IsPassed()) {
			break;
		}
	}
	for (; CurrentStep > 0; CurrentStep--) {
		CurrentStep--;
		if (Steps[CurrentStep]->IsPassed()) {
			CurrentStep++;
			break;
		}
		CurrentStep++;
	}
	//Tick step
	Steps[CurrentStep]->Tick.Broadcast();
	//Instruction
	SetInstruction(Steps[CurrentStep]->Instruction);
	//Highlight
	OnHighlightChange(Highlighted);
}

void ATutorialManager::AddTutorialStep(UTutorialStep* Step)
{
	Steps.Add(Step);
}

void ATutorialManager::SetPassed(bool Value)
{
	Steps[CurrentStep]->Passed = Value;
}

void ATutorialManager::SetPassedIfValid(AActor* Actor)
{
	SetPassed(IsValid(Actor));
}

void ATutorialManager::SetHighlight(AActor* Actor)
{
	if(IsValid(Actor)) Highlighted = Actor;
}

bool ATutorialManager::IsGrabbingClass(TSubclassOf<AActor> ActorClass)
{
	auto* Grabber = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetComponentByClass<UGrabber>();
	if (!Grabber->IsGrabbing()) return false;
	return Grabber->GetGrabbed()->GetOwner()->IsA(ActorClass);
}

TArray<AActor*> ATutorialManager::GetGrabbingClass(TSubclassOf<AActor> ActorClass)
{
	auto* Grabber = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetComponentByClass<UGrabber>();
	if (!Grabber->IsGrabbing()) return {};
	if (!Grabber->GetGrabbed()->GetOwner()->IsA(ActorClass)) return {};
	return { Grabber->GetGrabbed()->GetOwner() };
}

TArray<AActor*> ATutorialManager::FilterPhase(const TArray<AActor*>& Actors, FString Phase)
{
	TArray<AActor*> Result;
	for (AActor* Actor : Actors) {
		auto* Ingredient = Cast<AIngredient>(Actor);
		if (Ingredient && Ingredient->GetPhase() == Phase) {
			Result.Add(Actor);
		}
	}
	return Result;
}

void ATutorialManager::FilterAbove(const TArray<AActor*>& Actors, TSubclassOf<AActor> AboveClass, TArray<AActor*>& ActorsTop, TArray<AActor*>& ActorsBottom)
{
	for (AActor* Actor : Actors) {
		TArray<AActor*> Attached;
		Actor->GetAttachedActors(Attached);
		for (AActor* Above : Attached) {
			if (Above->IsA(AboveClass)) {
				ActorsBottom.Add(Actor);
				ActorsTop.Add(Above);
				break;
			}
		}
	}
}

void ATutorialManager::FilterBelow(const TArray<AActor*>& Actors, TSubclassOf<AActor> BelowClass, TArray<AActor*>& ActorsTop, TArray<AActor*>& ActorsBottom)
{
	for (AActor* Actor : Actors) {
		AActor* Parent = Actor->GetAttachParentActor();
		if (Parent && Parent->IsA(BelowClass)) {
			ActorsBottom.Add(Parent);
			ActorsTop.Add(Actor);
		}
	}
}

TArray<AActor*> ATutorialManager::GetActorOnActor(TSubclassOf<AActor> Bottom, TSubclassOf<AActor> Top)
{
	TArray<AActor*> ReturnActors;
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Bottom, OutActors);
	for (AActor* BottomActor : OutActors) {
		TArray<AActor*> OutActorsTop;
		BottomActor->GetAttachedActors(OutActorsTop);
		for (AActor* TopActor : OutActorsTop) {
			if (TopActor->IsA(Top)) {
				ReturnActors.Add(BottomActor);
				break;
			}
		}
	}
	return ReturnActors;
}

AActor* ATutorialManager::GetNearestActor(TArray<AActor*> Actors)
{
	FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetPawnViewLocation();
	float MinDistance = INFINITY;
	AActor* MinActor = nullptr;
	for (AActor* Actor : Actors) {
		float NewDistance = FVector::Distance(PlayerLocation, Actor->GetActorLocation());
		if (NewDistance < MinDistance) {
			MinDistance = NewDistance;
			MinActor = Actor;
		}
	}
	return MinActor;
}

UTutorialStep* UTutorialStep::TutorialStep(ATutorialManager* TutorialManager, FText Instruction, bool Essential, bool Remember)
{
	UTutorialStep* Node = NewObject<UTutorialStep>();
	if (Node)
	{
		Node->TutorialManager = TutorialManager;
		Node->Instruction = Instruction;
		Node->Essential = Essential;
		Node->Remember = Remember;
	}
	return Node;
}