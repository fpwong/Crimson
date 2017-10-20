// Fill out your copyright notice in the Description page of Project Settings.

#include "Crimson.h"
#include "VitalCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"


// Sets default values
AVitalCharacter::AVitalCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanReload = 1;
	bCanRechargeShield = 0;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void AVitalCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVitalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// recharge shield if it is less than max
	if (HasAuthority() && bCanRechargeShield && Shield < MaxShield)
	{
		Shield = FMath::Min(MaxShield, Shield + MaxShield * 0.25f * DeltaTime);
	}
}

// Called to bind functionality to input
void AVitalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVitalCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVitalCharacter, Health);
	DOREPLIFETIME(AVitalCharacter, Ammo);
	DOREPLIFETIME(AVitalCharacter, Shield);
	DOREPLIFETIME(AVitalCharacter, bDead);
}

float AVitalCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Server_ApplyDamage(Damage);
	return Damage;
}

void AVitalCharacter::Server_Reload_Implementation()
{
	if (const UWorld* World = GetWorld())
	{
		if (bCanReload)
		{
			bCanReload = 0;
			World->GetTimerManager().SetTimer(TimerHandle_ReloadTimerExpired, this, &AVitalCharacter::ReloadTimerExpired, 1.f);
		}
	}
}

bool AVitalCharacter::Server_Reload_Validate()
{
	return true;
}

void AVitalCharacter::Client_PlayReloadSound_Implementation()
{
	UGameplayStatics::PlaySound2D(this, ReloadSound);
}

bool AVitalCharacter::Client_PlayReloadSound_Validate()
{
	return true;
}

void AVitalCharacter::ReloadTimerExpired()
{
	bCanReload = 1;
	Ammo = MaxAmmo;

	Client_PlayReloadSound();
}

void AVitalCharacter::ShieldTimerExpired()
{
	bCanRechargeShield = 1;
}

void AVitalCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Health = MaxHealth;
	Ammo = MaxAmmo;
	Shield = MaxShield;
}

void AVitalCharacter::Multicast_OnDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AVitalCharacter::Multicast_OnDeath_Validate()
{
	return true;
}

void AVitalCharacter::Server_ApplyDamage_Implementation(float Damage)
{
	bCanRechargeShield = 0;

	float DamageAfterShield = FMath::Max(0.f, Damage - Shield);

	Shield = FMath::Max(0.f, Shield - Damage);

	Health = FMath::Max(0.f, Health - DamageAfterShield);

	if (Health == 0)
	{
		bDead = true;
		Multicast_OnDeath();

		if (AAIController* AIController = Cast<AAIController>(GetController()))
		{
			AIController->GetBrainComponent()->StopLogic("Dead");
		}
	}

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle_ShieldTimerExpired, this, &AVitalCharacter::ShieldTimerExpired, 2.f);
	}
}

bool AVitalCharacter::Server_ApplyDamage_Validate(float Damage)
{
	return true;
}

void AVitalCharacter::Server_UseAmmo_Implementation(int Amount)
{
	Ammo -= Amount;
}

bool AVitalCharacter::Server_UseAmmo_Validate(int Amount)
{
	return true;
}