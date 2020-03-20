// Fill out your copyright notice in the Description page of Project Settings.


#include "SY_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASY_Projectile::ASY_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(1.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASY_Projectile::OnHit);		// set up a notification for when this component hits something blocking

	RootComponent = CollisionComp;
	//CollisionComp->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	//CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	//CollisionComp->SetCollisionObjectType(ECC_WorldStatic);
	//CollisionComp->SetCollisionObjectType(TEXT("OverlapOnlyPawn"));


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	Mesh_Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Mesh_Arrow->SetupAttachment(CollisionComp);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ARROW(TEXT("/SYCharacter/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Shape_Pipe.Shape_Pipe"));
	if (SM_ARROW.Succeeded())
	{
	
		Mesh_Arrow->SetStaticMesh(SM_ARROW.Object);

		Mesh_Arrow->SetRelativeScale3D(FVector(0.3f, 1.0f, 0.3f));
		Mesh_Arrow->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_COMBOONE(TEXT("/Game/Effect/Result/Making/Physic/18_Char_ArrowHit/P_ArrowHit.P_ArrowHit"));
	if (P_COMBOONE.Succeeded())
	{
		ArrowParticle = P_COMBOONE.Object;

	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_TRAIL(TEXT("/Game/Particles/P_Arrow_Trail.P_Arrow_Trail"));
	if (P_TRAIL.Succeeded())
	{
		ArrowTrail = P_TRAIL.Object;

	}

	
}

// Called when the game starts or when spawned
void ASY_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASY_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASY_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL)&& OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	}
	FDamageEvent DamageEvent;
	if (OtherActor!= nullptr)
	{
	
		OtherActor->TakeDamage(10.0f, DamageEvent, GetWorld()->GetFirstPlayerController(), GetWorld()->GetFirstPlayerController()->GetPawn());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ArrowParticle, Hit.Location, FRotator());

	
	}
	Destroy();
}
void ASY_Projectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}