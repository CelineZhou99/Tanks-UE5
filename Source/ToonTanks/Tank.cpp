// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);
}

// calling super to not miss any parent functionality as we are overriding
void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Called to bind functionality to input
    PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move);
    PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
    Super::BeginPlay();

    // get the controller of the pawn
    // GetController returns AController (parent) which points to a APlayerController (child)
    // needs casting
    TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    // X = Value * Deltatime * Speed
    // get the deltatime
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaLocation.X = Value * DeltaTime * Speed;
    // enable sweeping
    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    // Yaw = Value * Deltatime * TurnRate
    float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaRotation.Yaw = Value * DeltaTime * TurnRate;

    AddActorLocalRotation(DeltaRotation, true);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // get the hit results under the cursor
    if (TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25, 12, FColor::Red);
        // rotate the turret based on the impact point of the hit result
        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    // hide tank instead of destroying
    // still be able to see the world through the camera attached to it
    SetActorHiddenInGame(true);
    // disable ticking
    SetActorTickEnabled(false);
    // set the tank alive status to false to stop turrets from firing
    bAlive = false;
}