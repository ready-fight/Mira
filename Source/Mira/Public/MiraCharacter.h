// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MiraCharacter.generated.h"

class AItemPickup;
class UInputAction;

UCLASS()
class MIRA_API AMiraCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Interaction")
	TObjectPtr<AItemPickup> NearbyItem;

public:
	AMiraCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetNearbyItem(AItemPickup* Item);
	void ClearNearbyItem(AItemPickup* Item);

protected:
	virtual void BeginPlay() override;
	void Interact();

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputAction> InteractAction;


};
