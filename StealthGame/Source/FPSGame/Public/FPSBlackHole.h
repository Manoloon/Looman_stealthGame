// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHole();

protected:

		UPROPERTY(VisibleAnyWhere, Category = "Component")
			UStaticMeshComponent * MeshComp;
		UPROPERTY(VisibleAnyWhere, Category = "Component")
			USphereComponent *	InnerSphereComp;
		UPROPERTY(VisibleAnyWhere, Category = "Component")
			USphereComponent *	OuterSphereComp;

	// Marked with UFUNCTION to BIND to OVerlap event
	UFUNCTION()
		void OverlapInnerSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResults);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
