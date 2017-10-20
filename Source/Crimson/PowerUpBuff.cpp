// Fill out your copyright notice in the Description page of Project Settings.

#include "Crimson.h"
#include "PowerUpBuff.h"


// Sets default values
APowerUpBuff::APowerUpBuff()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;
}

// Called when the game starts or when spawned
void APowerUpBuff::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		ApplyBuff(AppliedCharacter);
		World->GetTimerManager().SetTimer(TimerHandle_BuffTimerExpired, this, &APowerUpBuff::BuffTimerExpired, BuffDuration);
		Multicast_PlayParticleSystem();
	}
}

void APowerUpBuff::BuffTimerExpired()
{
	RemoveBuff(AppliedCharacter);
}

void APowerUpBuff::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APowerUpBuff, AppliedCharacter);
}

void APowerUpBuff::OnParticleRep()
{
	if (UWorld* World = GetWorld())
	{
		SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAttached(ParticleSystemClass, AppliedCharacter->GetRootComponent());
	}
}

void APowerUpBuff::ApplyBuff_Implementation(ACrimsonPawn* Character)
{
}

void APowerUpBuff::RemoveBuff_Implementation(ACrimsonPawn* Character)
{
	Multicast_DestroyParticleSystem();
}

void APowerUpBuff::Multicast_PlayParticleSystem_Implementation()
{
	if (UWorld* World = GetWorld())
	{
		SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAttached(ParticleSystemClass, AppliedCharacter->GetRootComponent());
	}
}

void APowerUpBuff::Multicast_DestroyParticleSystem_Implementation()
{
	if (SpawnedParticleSystem)
		SpawnedParticleSystem->DestroyComponent();
}