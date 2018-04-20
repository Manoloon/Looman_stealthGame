// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();

	// mision complete
	void MissionComplete(APawn* InstigatorPawn);
	
	// this function could be see on blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spectator")
		TSubclassOf<AActor>SpectatorViewpointClass;
};



