// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

class ACrimsonPawn;

UCLASS()
class CRIMSON_API APowerUp : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* Collision;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ParticleSystem")
	class UParticleSystem* ParticleSystemClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ParticleSystem")
	FColor ParticleColor = FColor::White;
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	

public:
	UFUNCTION(NetMulticast, reliable)
	void Multicast_PlayParticleSystem();

	UFUNCTION(Server, reliable, WithValidation)
	void Server_Activate(ACrimsonPawn* Character);

	UFUNCTION(BlueprintNativeEvent)
	void Activate(ACrimsonPawn* Character);
};
