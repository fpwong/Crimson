// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CrimsonController.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSON_API ACrimsonController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACrimsonController();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver();

	UFUNCTION(NetMulticast, reliable)
	void Multicast_GameOver();

	virtual void Tick(float DeltaSeconds) override;

};
