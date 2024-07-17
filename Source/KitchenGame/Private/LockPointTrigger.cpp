// Fill out your copyright notice in the Description page of Project Settings.


#include "LockPointTrigger.h"

// Sets default values
ALockPointTrigger::ALockPointTrigger() : VisibilityTimer(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALockPointTrigger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALockPointTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	VisibilityTimer -= DeltaTime;
	if(VisibilityTimer < 0.0f) {
		VisibilityTimer = 0.0f;
	}
	Splat->SetVisibility(IsSplat && VisibilityTimer > 0);
	Visual->SetVisibility(!IsSplat && VisibilityTimer > 0);
}

void ALockPointTrigger::SetEnableVisual(bool IsSauce) {
	VisibilityTimer = 0.2f;
	IsSplat = IsSauce;
}

