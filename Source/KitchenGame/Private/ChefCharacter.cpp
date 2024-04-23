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
	//Punch
	ForceDirection = Camera->GetForwardVector();
	auto PunchHitActors = TraceAttack(Camera->GetComponentLocation(), Camera->GetComponentLocation() + Camera->GetForwardVector() * PunchRange);
	//Kick
	ForceDirection = KickOrigin->GetForwardVector() + FVector{ 0.0f, 0.0f, 0.2f };
	ForceDirection.Normalize();
	auto KickHitActors = TraceAttack(KickOrigin->GetComponentLocation(), KickOrigin->GetComponentLocation() + KickOrigin->GetForwardVector() * KickRange);
	if (PunchHitActors.IsEmpty() != KickHitActors.IsEmpty()) {
		if (!PunchHitActors.IsEmpty()) {
			HitActors = PunchHitActors;
			Branches = EAttackType::Punch;
			String = "Punch";
			return true;
		}
		else {
			HitActors = KickHitActors;
			Branches = EAttackType::Kick;
			String = "Kick";
			return true;
		}
	}
	else {
		if (Camera->GetRelativeRotation().Pitch > -25) {
			HitActors = PunchHitActors;
			Branches = EAttackType::Punch;
			if (HitActors.IsEmpty()) return false;
			String = "Punch";
			return true;
		}
		else {
			HitActors = KickHitActors;
			Branches = EAttackType::Kick;
			if (HitActors.IsEmpty()) return false;
			String = "Kick";
			return true;
		}
	}
	return false;
}

void AChefCharacter::ResetAttackCooldown()
{
	AttackCooldownTimer = AttackCooldown;
}

TArray<AActor*> AChefCharacter::TraceAttack(FVector Start, FVector End)
{
	FCollisionQueryParams Params(TEXT("TraceAttack"));
	Params.AddIgnoredActor(this);
	TArray<FHitResult> OutHits;
	bool Result = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(20.0f), Params);
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

