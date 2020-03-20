// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SYCharacter.h"
#include "GameFramework/Actor.h"
#include "SY_Projectile.generated.h"

UCLASS()
class SYCHARACTER_API ASY_Projectile : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh_Arrow;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystem* ArrowParticle;
	UPROPERTY(VisibleAnywhere, Category = Effect)
		UParticleSystem* ArrowTrail;

public:	
	// Sets default values for this actor's properties
	ASY_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void FireInDirection(const FVector& ShootDirection);
};
