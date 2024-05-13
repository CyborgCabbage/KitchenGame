// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChefCharacter.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Then,
	Punch,
	Kick
};

UCLASS()
class KITCHENGAME_API AChefCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChefCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float AttackCooldown;

	UPROPERTY(EditDefaultsOnly)
	float PunchRange;

	UPROPERTY(EditDefaultsOnly)
	float KickRange;

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	bool CanAttack(const USceneComponent* KickOrigin, FString& String, TArray<AActor*>& HitActors, FVector& ForceDirection, EAttackType& Branches);

	UFUNCTION(BlueprintCallable)
	void ResetAttackCooldown();

private:
	float AttackCooldownTimer;
	TArray<AActor*> TraceAttack(FVector Start, FVector End);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
