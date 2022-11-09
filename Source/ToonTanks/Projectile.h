// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent *ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UProjectileMovementComponent *ProjectileMovementComponent;

	// callback function for oncomponenthit
	// hitcomp - the component doing the hitting
	// otheractor - the actor that got hit
	// othercomp - the other component that was hit
	// normalimpulse - direction and magnitude of the on hit impulse
	// hit - the hit result containing info
	// has to be ufunction for it to work
	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem *HitParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystemComponent *TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase *LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
};
