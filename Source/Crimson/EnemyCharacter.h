// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VitalCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSON_API AEnemyCharacter : public AVitalCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();
	
	virtual void BeginPlay() override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void HitEnabledTimerExpired();

	FTimerHandle TimerHandle_HitEnabled;
	uint32 bCanHit : 1;
};
