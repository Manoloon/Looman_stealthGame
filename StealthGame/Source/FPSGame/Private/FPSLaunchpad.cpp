// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchpad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchpad::AFPSLaunchpad()
{
 	// trigger Box
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = BoxComp;

	// mesh launchpad
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);	

	// bind event 
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchpad::OverlapLaunchPad);

	// set variables
	Force = 1500;
	LaunchPitchAngle = 35.0f;

	// Arrow component
	ArrowDir = CreateAbstractDefaultSubobject<UArrowComponent>(TEXT("ArrowDir"));
	ArrowDir->SetupAttachment(BoxComp);
		
}

void AFPSLaunchpad::OverlapLaunchPad(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & bSweepResults)
{
	// Make Rotator with our pitch
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * Force;

	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if(OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);
		PlayEffects();
		UE_LOG(LogTemp, Warning, TEXT("CHARACTER"));
	}
	else if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		PlayEffects();
		UE_LOG(LogTemp, Warning, TEXT("comp"));
	}
}

void AFPSLaunchpad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UseFX, GetActorLocation());
}

