// Fill out your copyright notice in the Description page of Project Settings.


#include "ChefCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AChefCharacter::AChefCharacter() : AttackCooldown(0), AttackCooldownTimer(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AChefCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AChefCharacter::CanAttack(const USceneComponent* KickOrigin, FString& String, TArray<AActor*>& HitActors, FVector& ForceDirection, EAttackType& Branches)
{
	String = "";
	ForceDirection = FVector::ZeroVector;
	Branches = EAttackType::Then;
	if (AttackCooldownTimer > 0) return false;
	auto* Camera = GetComponentByClass<UCameraComponent>();
	if (Camera->GetRelativeRotation().Pitch > -25) {
		//Punch
		ForceDirection = Camera->GetForwardVector();
		Branches = EAttackType::Punch;
		HitActors = TraceAttack(Camera->GetComponentLocation(), Camera->GetComponentLocation() + Camera->GetForwardVector() * PunchRange);
		if (HitActors.IsEmpty()) return false;
		String = "Punch";
		return true;
	}
	else {
		//Kick
		ForceDirection = KickOrigin->GetForwardVector() + FVector{ 0.0f, 0.0f, 0.2f };
		ForceDirection.Normalize();
		Branches = EAttackType::Kick;
		HitActors = TraceAttack(KickOrigin->GetComponentLocation(), KickOrigin->GetComponentLocation() + KickOrigin->GetForwardVector() * KickRange);
		if (HitActors.IsEmpty()) return false;
		String = "Kick";
		return true;
	}
	return false;
}

void AChefCharacter::ResetAttackCooldown()
{
	AttackCooldownTimer = AttackCooldown;
}

TArray<AActor*> AChefCharacter::TraceAttack(FVector Start, FVector End)
{
	FCollisionQueryParams Params(TEXT("PlayerAttack"));
	Params.AddIgnoredActor(this);
	TArray<FHitResult> OutHits;
	bool Result = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_GameTraceChannel2 /*Attack Trace*/, FCollisionShape::MakeSphere(20.0f), Params);
	TArray<AActor*> HitActors;
	for (const auto& Hit : OutHits) {
		if(!Hit.bBlockingHit) {
			HitActors.Add(Hit.GetActor());
		}
	}

	return HitActors;
}

// Called every frame
void AChefCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AttackCooldownTimer -= DeltaTime;
	if (AttackCooldownTimer < 0) {
		AttackCooldownTimer = 0;
	}
}

// Called to bind functionality to input
void AChefCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

