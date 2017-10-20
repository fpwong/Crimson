// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CrimsonPawn.h"
#include "GameFramework/Actor.h"
#include "PowerUpBuff.generated.h"


UCLASS()
class CRIMSON_API APowerUpBuff : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn), ReplicatedUsing = OnParticleRep)
	ACrimsonPawn* AppliedCharacter;

	class UParticleSystemComponent* SpawnedParticleSystem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated)
	float BuffDuration = 5.f;

	// Sets default values for this actor's properties
	APowerUpBuff();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ParticleSystem")
	class UParticleSystem* ParticleSystemClass;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Powerup")
	void ApplyBuff(ACrimsonPawn* Character);

	UFUNCTION(BlueprintNativeEvent)
	void RemoveBuff(ACrimsonPawn* Character);

	void BuffTimerExpired();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FTimerHandle TimerHandle_BuffTimerExpired;

	UFUNCTION(NetMulticast, reliable)
	void Multicast_PlayParticleSystem();

	UFUNCTION()
	void OnParticleRep();

	UFUNCTION(NetMulticast, reliable)
	void Multicast_DestroyParticleSystem();
};
