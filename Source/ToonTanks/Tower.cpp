// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Tank.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }
}

void ATower::CheckFireCondition()
{
    if (!Tank)
    {
        return;
    }
    if (InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::InFireRange()
{
    if (Tank)
    {
        // find the distance to the tank
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        // check to see if the tank is in range
        // if in range, rotate turret towards the tank
        if (Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}

void ATower::HandleDestruction()
{
    // make sure the visual/sound effects are called
    Super::HandleDestruction();
    Destroy();
}
