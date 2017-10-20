// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "VitalCharacter.generated.h"

UCLASS()
class CRIMSON_API AVitalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "VitalStats", EditAnywhere, BlueprintReadWrite, Replicated)
	float Health;

	UPROPERTY(Category = "VitalStats", EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100;

	UPROPERTY(Category = "VitalStats", EditAnywhere, BlueprintReadWrite, Replicated)
	int Ammo;

	UPROPERTY(Category = "VitalStats", EditDefaultsOnly, BlueprintReadOnly)
	int MaxAmmo = 25;

	UPROPERTY(Category = "VitalStats", EditAnywhere, BlueprintReadWrite, Replicated)
	float Shield;

	UPROPERTY(Category = "VitalStats", EditDefaultsOnly, BlueprintReadOnly)
	float MaxShield = 25;

	UPROPERTY(Category = "VitalStats", EditAnywhere, BlueprintReadWrite, Replicated)
	uint32 bDead : 1;

	// Sets default values for this character's properties
	AVitalCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Server, reliable, WithValidation)
	void Server_ApplyDamage(float Damage);

	UFUNCTION(Server, reliable, WithValidation)
	void Server_UseAmmo(int Amount);

	UFUNCTION(Server, reliable, WithValidation)
	void Server_Reload();

	/** Sound to play on reload */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* ReloadSound;

	UFUNCTION(Client, reliable, WithValidation)
	void Client_PlayReloadSound();

	void ReloadTimerExpired();

	/* Flag to control reloading */
	uint32 bCanReload : 1;

	uint32 bCanRechargeShield : 1;
	void ShieldTimerExpired();

	UFUNCTION(NetMulticast, reliable, WithValidation)
	void Multicast_OnDeath();



	virtual void OnConstruction(const FTransform& Transform) override;

private:
	FTimerHandle TimerHandle_ReloadTimerExpired;
	FTimerHandle TimerHandle_ShieldTimerExpired;
};
