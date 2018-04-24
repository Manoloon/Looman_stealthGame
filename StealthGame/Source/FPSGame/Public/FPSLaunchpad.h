// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchpad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class FPSGAME_API AFPSLaunchpad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchpad();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UBoxComponent* BoxComp;
	UPROPERTY(VisibleAnywhere, Category = "Component")
		UArrowComponent* ArrowDir;
	UPROPERTY(EditInstanceOnly, Category = "Data")
		float Force;
	UPROPERTY(EditInstanceOnly, Category ="Data")
		float LaunchPitchAngle;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem*  UseFX;

	UFUNCTION()
		void OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& bSweepResults);

	// play particle effect
	void PlayEffects();
};