#pragma once

#include "CoreMinimal.h"
#include "MiraCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"
#include "MiraEnemy.generated.h"

class AAIController;

UCLASS()
class MIRA_API AMiraEnemy : public ACharacter
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackCooldown = 2.0f;

    enum class EMoveState : uint8
    {
        Chasing,
        Attacking,
        ReturningHome,
        AtHome
    };

    EMoveState MoveState = EMoveState::AtHome;

    FVector StartLocation;
    FRotator StartRotation;

    FTimerHandle DistanceTimerHandle;
    FTimerHandle AttackCooldownTimerHandle;

    bool bIsAttacking = false;
    bool bIsReturningRotation = false;
    bool bAttackOnCooldown = false;

    AMiraCharacter* Player = nullptr;
    AAIController* AI = nullptr;

    void UpdatePlayerDistance();
    float CheckPlayerDistance();

    void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
    bool IsAttacking() { return bIsAttacking; }

public:
    AMiraEnemy();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
    UFUNCTION(BlueprintCallable)
    void NotifyAttackFinished();
    UFUNCTION(BlueprintCallable)
    void NotifyDealDamage();
    UFUNCTION(BlueprintImplementableEvent, Category = "Combat")
    void OnAttack();
    void FinishAttackCooldown();
    virtual void BeginPlay() override;

};