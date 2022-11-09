// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor *DeadActor)
{
    // check if dead actor is the tank
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower *DestroyedTower = Cast<ATower>(DeadActor)) // if cast is successful means it is a tower
    {
        DestroyedTower->HandleDestruction();
        // decrement the num of towers
        NumTowers--;
        if (NumTowers == 0)
        {
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    NumTowers = GetNumTowersCount();
    // pointer to tank
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    // pointer to player controller
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnabledTimerHandle;
        FTimerDelegate PlayerEnabledTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true);

        GetWorldTimerManager().SetTimer(PlayerEnabledTimerHandle, PlayerEnabledTimerDelegate, StartDelay, false);
    }
}

int32 AToonTanksGameMode::GetNumTowersCount()
{
    TArray<AActor *> Towers;
    // get all actors with class ATower
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    // return the count
    return Towers.Num();
}