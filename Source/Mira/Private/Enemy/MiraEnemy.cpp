// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MiraEnemy.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

#include "Kismet/GameplayStatics.h"

#include "TimerManager.h"

void AMiraEnemy::UpdatePlayerDistance()
{
	if (!Player || !AI)
	{
		return;
	}

	const float Distance = CheckPlayerDistance();

	if (Distance <= 150.0f && !Player->IsDead())
	{

		bIsReturningRotation = false;

		if (MoveState != EMoveState::Attacking) {
			MoveState = EMoveState::Attacking;
			AI->StopMovement();
		}

		if (!bIsAttacking) {
			OnAttack();
			bIsAttacking = true;
		}


	}
	else if (Distance <= 1000.0f && !Player->IsDead())
	{
		bIsReturningRotation = false;

		if (MoveState != EMoveState::Chasing)
		{
			MoveState = EMoveState::Chasing;
			AI->MoveToActor(Player, 100.0f);
		}
	}
	else if (MoveState != EMoveState::ReturningHome && MoveState != EMoveState::AtHome)
	{
		MoveState = EMoveState::ReturningHome;
		AI->MoveToLocation(StartLocation, 50.0f);
	}
}

float AMiraEnemy::CheckPlayerDistance()
{
	return FVector::Dist(GetActorLocation(), Player->GetActorLocation());
}

void AMiraEnemy::OnMoveFinished(
	FAIRequestID RequestID,
	const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		switch (MoveState) {
			case EMoveState::ReturningHome:
				MoveState = EMoveState::AtHome;
				bIsReturningRotation = true;
				break;
			default:
				MoveState = EMoveState::ReturningHome;
		}
	}
	else
	{
		MoveState = EMoveState::ReturningHome;
	}
}

AMiraEnemy::AMiraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMiraEnemy::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AMiraCharacter>(
		UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)
	);
	AI = Cast<AAIController>(GetController());

	StartLocation = this->GetActorLocation();
	StartRotation = this->GetActorRotation();

	GetWorldTimerManager().SetTimer(
		DistanceTimerHandle,
		this,
		&AMiraEnemy::UpdatePlayerDistance,
		0.25f,
		true
	);

	if (AI && AI->GetPathFollowingComponent())
	{
		AI->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
			this, &AMiraEnemy::OnMoveFinished
		);
	}
	
}

// Called every frame
void AMiraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsReturningRotation)
	{
		return;
	}

	const FRotator NewRotation = FMath::RInterpTo(
		GetActorRotation(),
		StartRotation,
		DeltaTime,
		5.0f
	);

	SetActorRotation(NewRotation);

	const float YawDifference = FMath::Abs(
		FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, StartRotation.Yaw)
	);

	if (YawDifference <= 2.0f)
	{
		SetActorRotation(StartRotation);
		bIsReturningRotation = false;
	}
}

// Called to bind functionality to input
void AMiraEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMiraEnemy::NotifyAttackFinished()
{
	bIsAttacking = false;
}

void AMiraEnemy::NotifyDealDamage() {
	Player->TakeDamage(10.0f);
}

