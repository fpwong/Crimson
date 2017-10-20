// Fill out your copyright notice in the Description page of Project Settings.

#include "Crimson.h"
#include "GameCoordinator.h"


void AGameCoordinator::PowerupTimerExpired()
{
	if (PowerupsToSpawn.Num() > 0)
	{
		//TSubclassOf<AActor> PowerupToSpawn = PowerupsToSpawn[FMath::Rand() % PowerupsToSpawn.Num() - 1];
		SpawnActorInRange(PowerupsToSpawn[FMath::Rand() % PowerupsToSpawn.Num()]);
	}
}

void AGameCoordinator::EnemyTimerExpired()
{
	for (int i = 0; i < EnemyCount; i++)
	{
		SpawnActorInRange(EnemyToSpawn);
	}
}

void AGameCoordinator::SpawnActorInRange(UClass* ActorClass)
{
	if (UWorld* World = GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Actor!"));
		FVector Location = FMath::RandPointInBox(SpawnBox->Bounds.GetBox());
		FRotator ZeroRotation = FRotator::ZeroRotator;
		World->SpawnActor<AActor>(ActorClass, Location, ZeroRotation);
	}
}

// Sets default values
AGameCoordinator::AGameCoordinator()
{
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PowerupRate = 10.f;
	EnemyRate = 2.f;
	EnemyCount = 1;
}

// Called when the game starts or when spawned
void AGameCoordinator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(TimerHandle_SpawnPowerup, this, &AGameCoordinator::PowerupTimerExpired, PowerupRate, true);
			World->GetTimerManager().SetTimer(TimerHandle_SpawnEnemy, this, &AGameCoordinator::EnemyTimerExpired, EnemyRate, true);
		}
	}
}

// Called every frame
void AGameCoordinator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

