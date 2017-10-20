// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameCoordinator.generated.h"

UCLASS()
class CRIMSON_API AGameCoordinator : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* SpawnBox;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UClass* EnemyToSpawn;

	float EnemyRate;
	int EnemyCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Test")
	TArray<UClass*> PowerupsToSpawn;
	float PowerupRate;

	void PowerupTimerExpired();
	void EnemyTimerExpired();

	void SpawnActorInRange(UClass* ActorClass);

	// Sets default values for this actor's properties
	AGameCoordinator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	FTimerHandle TimerHandle_SpawnEnemy;
	FTimerHandle TimerHandle_SpawnPowerup;
};
