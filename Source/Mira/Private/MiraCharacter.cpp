// Fill out your copyright notice in the Description page of Project Settings.


#include "MiraCharacter.h"

#include "EnhancedInputComponent.h"
#include "ItemPickup.h"

// Sets default values
AMiraCharacter::AMiraCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMiraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMiraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMiraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent && InteractAction)
	{
		EnhancedInputComponent->BindAction(
			InteractAction,
			ETriggerEvent::Started,
			this,
			&AMiraCharacter::Interact
		);
	}

	if (EnhancedInputComponent && AttackAction)
	{
		EnhancedInputComponent->BindAction(
			AttackAction,
			ETriggerEvent::Started,
			this,
			&AMiraCharacter::Attack
		);
	}
}

void AMiraCharacter::SetNearbyItem(AItemPickup* Item)
{
	if (IsValid(Item))
	{
		NearbyItem = Item;
	}
}

void AMiraCharacter::ClearNearbyItem(AItemPickup* Item)
{
	if (NearbyItem == Item)
	{
		NearbyItem = nullptr;
	}
}

void AMiraCharacter::Interact()
{	if (IsValid(NearbyItem))
	{
		NearbyItem->Interact(this);
	}
}

void AMiraCharacter::Attack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		OnAttack();
	}
}

void AMiraCharacter::NotifyAttackAnimationFinished()
{
	bIsAttacking = false;
}