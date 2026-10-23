// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class NETWORKTANKARENA_API ATankProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATankProjectile();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Settings", meta = (ExposeOnSpawn = true))
	float ProjectilePower = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	float MaxProjectileSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	float InitialProjectileSpeed = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	bool bRotateToVelocity = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
