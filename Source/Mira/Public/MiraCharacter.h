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
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(VisibleInstanceOnly, Category = "Interaction")
	TObjectPtr<AItemPickup> NearbyItem;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputAction> AttackAction;

private:
	float CurrentHealth = MaxHealth;
	bool bIsDead;
	bool bIsAttacking = false;

public:
	AMiraCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetNearbyItem(AItemPickup* Item);
	void ClearNearbyItem(AItemPickup* Item);
	void Heal(float Health);
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() { return bIsDead; }
	float GetCurrentHealath() { return CurrentHealth; }

protected:
	virtual void BeginPlay() override;
	void Interact();
	void Attack();

	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void OnAttack();

	UFUNCTION(BlueprintCallable)
	void NotifyAttackAnimationFinished();
};
