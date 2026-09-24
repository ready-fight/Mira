#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
        Idle,
        Chasing,
        ReturningHome,
        AtHome
    };

    EMoveState MoveState = EMoveState::AtHome;

    FVector StartLocation;
    FRotator StartRotation;
    FTimerHandle DistanceTimerHandle;

    bool bIsReturningRotation = false;
    ACharacter* Player = nullptr;
    AAIController* AI = nullptr;

    void UpdatePlayerDistance();
    float CheckPlayerDistance();
    void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
    AMiraEnemy();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;
};