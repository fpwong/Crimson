// Fill out your copyright notice in the Description page of Project Settings.

#include "Crimson.h"
#include "EnemyCharacter.h"
#include "CrimsonPawn.h"




AEnemyCharacter::AEnemyCharacter()
{
	bCanHit = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
}

void AEnemyCharacter::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	/*if (HasAuthority() && bCanHit)
	{
		if (ACrimsonPawn* Player = Cast<ACrimsonPawn>(Other))
		{
			if (UWorld* World = GetWorld())
			{
				FDamageEvent DamageEvent;
				Player->TakeDamage(10, DamageEvent, GetController(), this);

				World->GetTimerManager().SetTimer(TimerHandle_HitEnabled, this, &AEnemyCharacter::HitEnabledTimerExpired, 1.f);
				bCanHit = false;
			}
		}
	}*/
}

void AEnemyCharacter::HitEnabledTimerExpired()
{
	bCanHit = true;
}
