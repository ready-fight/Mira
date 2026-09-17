// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "MiraCharacter.h"

AItemPickup::AItemPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupSphere = CreateDefaultSubobject<USphereComponent>(
		TEXT("PickupSphere")
	);

	SetRootComponent(PickupSphere);

	PickupSphere->InitSphereRadius(100.0f);
	PickupSphere->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly
	);
	PickupSphere->SetCollisionObjectType(ECC_WorldDynamic);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(
		ECC_Pawn,
		ECR_Overlap
	);
	PickupSphere->SetGenerateOverlapEvents(true);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("ItemMesh")
	);

	ItemMesh->SetupAttachment(PickupSphere);
	ItemMesh->SetCollisionEnabled(
		ECollisionEnabled::NoCollision
	);

	RotatingMovement =
		CreateDefaultSubobject<URotatingMovementComponent>(
			TEXT("RotatingMovement")
		);

	RotatingMovement->RotationRate = FRotator(
		0.0f,
		60.0f,
		0.0f
	);

}

void AItemPickup::BeginPlay()
{
	Super::BeginPlay();

	PickupSphere->OnComponentBeginOverlap.AddDynamic(
		this,
		&AItemPickup::OnPickupSphereBeginOverlap
	);

	PickupSphere->OnComponentEndOverlap.AddDynamic(
		this,
		&AItemPickup::OnPickupSphereEndOverlap
	);
}

void AItemPickup::OnPickupSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	AMiraCharacter* Character =
		Cast<AMiraCharacter>(OtherActor);

	if (Character)
	{
		Character->SetNearbyItem(this);
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("%s overlapped pickup %s"),
		*Character->GetName(),
		*GetName()
	);

}

void AItemPickup::OnPickupSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex
)
{
	AMiraCharacter* Character =
		Cast<AMiraCharacter>(OtherActor);

	if (Character)
	{
		Character->ClearNearbyItem(this);
	}
}

void AItemPickup::Interact(AMiraCharacter* Character)
{
	if (!IsValid(Character))
	{
		return;
	}

	OnPickedUp();
	Character->ClearNearbyItem(this);
	Destroy();
}