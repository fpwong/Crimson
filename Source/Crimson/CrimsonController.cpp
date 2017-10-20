// Fill out your copyright notice in the Description page of Project Settings.

#include "Crimson.h"
#include "CrimsonController.h"
#include "CrimsonPawn.h"




ACrimsonController::ACrimsonController()
{
	bShowMouseCursor = true;
}

void ACrimsonController::Multicast_GameOver_Implementation()
{
	OnGameOver();
}

void ACrimsonController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACrimsonPawn::StaticClass(), FoundActors);

		bool anyAlive = false;

		for (AActor* Actor : FoundActors)
		{
			if (ACrimsonPawn* Player = Cast<ACrimsonPawn>(Actor))
			{
				if (Player->Health > 0)
				{
					anyAlive = true;
					break;
				}
			}
		}

		if (!anyAlive)
		{
			Multicast_GameOver();
		}
	}
}
