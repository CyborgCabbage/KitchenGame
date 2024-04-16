// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialManager.h"

// Sets default values
ATutorialManager::ATutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsActive = false;
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
	OnTutorialStepDelegate.ExecuteIfBound();
}

void ATutorialManager::SetTutorialDelegate(FOnTutorialStepSignature Delegate) {
	OnTutorialStepDelegate = Delegate;
}
