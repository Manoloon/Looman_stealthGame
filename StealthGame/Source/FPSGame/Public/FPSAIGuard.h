// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState: uint8 
{
Idle,
Suspicious,
Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UPawnSensingComponent* PawnSensingComp;
	
	// Challenge 3
	UPROPERTY(EditInstanceOnly, Category = "Route", meta = (EditCondition = "bPatrolOn"))
		AActor* PatrolPoint1;
	UPROPERTY(EditInstanceOnly, Category = "Route", meta = (EditCondition = "bPatrolOn"))
		AActor* PatrolPoint2;
	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrolOn;
	// aca se setea el punto en el que estoy.
	AActor* CurrentPatrolPoint;

	void MoveToNextPatrolPoint();

	FRotator OriginalRotation;
	FTimerHandle  TimerHandle_ResetOrientation;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);
	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);
	UFUNCTION()
		void ResetOrientation();
	
	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI STATE")
		void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};
