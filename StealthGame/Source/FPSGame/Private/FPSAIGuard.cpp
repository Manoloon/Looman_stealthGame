// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));	
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
	//binding
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
	if (bPatrolOn)
	{	
		MoveToNextPatrolPoint();
	}
	else
	{
	GuardState = EAIState::Idle;
	}
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if(CurrentPatrolPoint == nullptr ||CurrentPatrolPoint == PatrolPoint1)
	{
		CurrentPatrolPoint = PatrolPoint2;
	}
	else 
	{
		CurrentPatrolPoint = PatrolPoint1;
	}
	UNavigationSystem::SimpleMoveToActor(GetController(),CurrentPatrolPoint);
}

void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	if(SeenPawn == nullptr)
	{
		return;
	}
	if(GuardState == EAIState::Alerted)
	{
		return;
	}
	SetGuardState(EAIState::Alerted);
	// patrolling stop
	if(bPatrolOn)
	{
		Controller->StopMovement();
	}
	// declaramos el juego perdido.
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->MissionComplete(SeenPawn, false);
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn * NoiseInstigator, const FVector& Location, float Volume)
{
	if( GuardState == EAIState::Alerted)
	{
		return;
	}
		DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
		// Con esto mejoramos el lookAtRotation de blueprints
		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();
		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.0f;
		NewLookAt.Roll = 0.0f;
		SetActorRotation(NewLookAt);
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation,3.0f);
		SetGuardState(EAIState::Suspicious);
		// patrolling stop
		if (bPatrolOn)
		{
			Controller->StopMovement();
		}
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
	// patrol state
	if (bPatrolOn)
	{
		MoveToNextPatrolPoint();
	}
	else
	{
	SetGuardState(EAIState::Idle);
	}
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;
	OnRep_GuardState();
}

// esto se usa para replicar y sincronizar la variable GUARDSTATE
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();
		if(DistanceToGoal<50)
		{
			MoveToNextPatrolPoint();
		}
	}
}


