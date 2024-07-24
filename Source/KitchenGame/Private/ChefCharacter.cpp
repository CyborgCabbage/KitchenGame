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

void DrawDebugSweptSphere(const UWorld* InWorld, FVector const& Start, FVector const& End, float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;
	float const HalfHeight = (Dist * 0.5f) + Radius;

	FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
}

void DrawDebugSphereTraceMulti(const UWorld* World, const FVector& Start, const FVector& End, float Radius, EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && OutHits.Last().bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			FVector const BlockingHitPoint = OutHits.Last().Location;
			DrawDebugSweptSphere(World, Start, BlockingHitPoint, Radius, TraceColor.ToFColor(true), bPersistent, LifeTime, 0);
			DrawDebugSweptSphere(World, BlockingHitPoint, End, Radius, TraceHitColor.ToFColor(true), bPersistent, LifeTime, 0);
		}
		else
		{
			// no hit means all red
			DrawDebugSweptSphere(World, Start, End, Radius, TraceColor.ToFColor(true), bPersistent, LifeTime, 0);
		}

		// draw hits
		for (int32 HitIdx = 0; HitIdx < OutHits.Num(); ++HitIdx)
		{
			FHitResult const& Hit = OutHits[HitIdx];
			DrawDebugPoint(World, Hit.ImpactPoint, 16.0f, (Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)), bPersistent, LifeTime);
		}
	}
}

TArray<AActor*> AChefCharacter::TraceAttack(FVector Start, FVector End)
{
	FCollisionQueryParams Params(TEXT("PlayerAttack"));
	Params.AddIgnoredActor(this);
	TArray<FHitResult> OutHits;
	bool Result = GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, ECC_GameTraceChannel2 /*Attack Trace*/, FCollisionShape::MakeSphere(20.0f), Params);
	//DrawDebugSphereTraceMulti(GetWorld(), Start, End, 20.0f, EDrawDebugTrace::ForDuration, Result, OutHits, FLinearColor(1,0,0), FLinearColor(0,1,0), 2.0f);
	TArray<AActor*> HitActors;
	for (const auto& Hit : OutHits) {
		AActor* A = Hit.GetActor();
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

