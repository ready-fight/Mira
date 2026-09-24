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

    bool bIsAttacking = false;
    bool bIsReturningRotation = false;
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
    UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
    void OnAttack();
    virtual void BeginPlay() override;
};