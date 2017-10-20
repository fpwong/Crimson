// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Crimson.h"
#include "CrimsonPawn.h"
#include "CrimsonProjectile.h"
#include "TimerManager.h"
#include "Animation/AnimBlueprint.h"

const FName ACrimsonPawn::MoveForwardBinding("MoveForward");
const FName ACrimsonPawn::MoveRightBinding("MoveRight");
const FName ACrimsonPawn::FireForwardBinding("FireForward");
const FName ACrimsonPawn::FireRightBinding("FireRight");

void ACrimsonPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACrimsonPawn, FireRate);
	DOREPLIFETIME(ACrimsonPawn, AmmoCost);
}

UCharacterMovementComponent* ACrimsonPawn::GetCharacterMovementComponent() const
{
	return Cast<UCharacterMovementComponent>(GetMovementComponent());
}

ACrimsonPawn::ACrimsonPawn()
{
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/Assets/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Load our skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Game/Assets/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->AddLocalOffset(FVector(0, 0, -88));
	GetMesh()->AddLocalRotation(FRotator(0, -90, 0));

	// Movement
	GetCharacterMovementComponent()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.3f;
	AmmoCost = 1;
	bCanFire = true;

	bReplicates = true;
	bReplicateMovement = true;
}

void ACrimsonPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up game play key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void ACrimsonPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bDead) return;

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection;
	AddMovementInput(Movement);

	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void ACrimsonPawn::FireShot(FVector FireDirection)
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			if (Ammo > 0)
			{
				const FRotator FireRotation = FireDirection.Rotation();

				// Spawn projectile at an offset from this pawn
				const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

				UWorld* const World = GetWorld();
				if (World)
				{
					// spawn the projectile
					Server_SpawnProjectile(SpawnLocation, FireRotation);
				}

				bCanFire = false;
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACrimsonPawn::ShotTimerExpired, FireRate);

				// try and play the sound if specified
				if (FireSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}

				bCanFire = false;
			}
			else
			{
				Server_Reload();
			}
		}
	}
}

void ACrimsonPawn::Server_SpawnProjectile_Implementation(FVector Location, FRotator Rotation)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		SpawnParams.Owner = this;

		ACrimsonProjectile* Projectile = World->SpawnActor<ACrimsonProjectile>(Location, Rotation, SpawnParams);
		Server_UseAmmo(AmmoCost);
	}
}

bool ACrimsonPawn::Server_SpawnProjectile_Validate(FVector Location, FRotator Rotation)
{
	return true;
}

void ACrimsonPawn::ShotTimerExpired()
{
	bCanFire = true;
}