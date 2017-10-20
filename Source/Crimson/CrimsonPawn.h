// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "VitalCharacter.h"
#include "CrimsonPawn.generated.h"

UCLASS(Blueprintable)
class ACrimsonPawn : public AVitalCharacter
{
	GENERATED_BODY()

public:
	ACrimsonPawn();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite, Replicated)
	float FireRate;

	/* How much ammo is used */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite, Replicated)
	int AmmoCost;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	UFUNCTION(Server, reliable, WithValidation)
	void Server_SpawnProjectile(FVector Location, FRotator Rotation);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UCharacterMovementComponent* GetCharacterMovementComponent() const;

private:
	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	
};

