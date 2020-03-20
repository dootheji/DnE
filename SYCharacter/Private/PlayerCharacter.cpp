// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "SYCharacter.h"
#include "DrawDebugHelpers.h"
#include "SY_Projectile.h"
#include "Camera/PlayerCameraManager.h"
#include "DnE_SaveGame.h"
#include "DnE_GameInstance.h"
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	PointLightFront = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightFront"));
	PointLightBack = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightBack"));
	SK_Scabbard = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Scabbard"));
	ST_Sword_S = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_Sword_S"));
	ST_Sword_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_Sword_L"));
	Bow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));

	//HitParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("TRAIL"));

	CameraBoom->SetupAttachment(GetCapsuleComponent());
	PointLightFront->SetupAttachment(GetMesh());
	PointLightBack->SetupAttachment(GetMesh());
	FollowCamera->SetupAttachment(CameraBoom);

	//ProjectileClass = ASY_Projectile::StaticClass();

	PointLightFront->SetIntensity(0.0f);
	PointLightFront->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	PointLightFront->SetAttenuationRadius(150.0f);
	PointLightBack->SetIntensity(0.0f);
	PointLightBack->SetRelativeLocation(FVector(0.0f, -60.0f, 80.0f));
	PointLightBack->SetAttenuationRadius(150.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	CameraBoom->TargetArmLength = 600;
	CameraBoom->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BELICA(TEXT("/SYCharacter/Sword_Elf/Models/SK_Elf.SK_Elf"));
	if (BELICA.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(BELICA.Object);
	}


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BOW(TEXT("/SYCharacter/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));
	if (BOW.Succeeded())
	{
	
		FName Bow_Socket(TEXT("Bow_pelvis_Sock"));;
		if (GetMesh()->DoesSocketExist(Bow_Socket))
		{

			Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Bow_Socket);
			Bow->SetSkeletalMesh(BOW.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SCABBARD(TEXT("/SYCharacter/Sword_Elf/Models/Wepons/SK_Scabbard.SK_Scabbard"));
	if (SCABBARD.Succeeded())
	{
		SK_Scabbard->SetupAttachment(GetMesh());
		SK_Scabbard->SetSkeletalMesh(SCABBARD.Object);
		SK_Scabbard->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
		SK_Scabbard->SetMasterPoseComponent(GetMesh());
	}

	FName sSocket(TEXT("Wepon_S"));
	if (SK_Scabbard->DoesSocketExist(sSocket))
	{

		static ConstructorHelpers::FObjectFinder<UStaticMesh> SWORDS(TEXT("/SYCharacter/Sword_Elf/Models/Wepons/ST_Sword_S.ST_Sword_S"));
		if (SWORDS.Succeeded())
		{
			ST_Sword_S->SetStaticMesh(SWORDS.Object);
		}
		ST_Sword_S->SetupAttachment(SK_Scabbard, sSocket);
	}

	FName lSocket(TEXT("Wepon_L"));
	if (SK_Scabbard->DoesSocketExist(lSocket))
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SWORDL(TEXT("/SYCharacter/Sword_Elf/Models/Wepons/ST_Sword_L.ST_Sword_L"));
		if (SWORDL.Succeeded())
		{
			ST_Sword_L->SetStaticMesh(SWORDL.Object);
		}
		ST_Sword_L->SetupAttachment(SK_Scabbard, lSocket);
	}


	//캐릭터 애니메이션

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/SYCharacter/Sword_Elf/Aurora/Animations/Use/Mon_Stun.Mon_Stun"));

	if (ATTACK_MONTAGE.Succeeded())
	{
		StunMontage = ATTACK_MONTAGE.Object;
	}


	static ConstructorHelpers::FClassFinder<UAnimInstance> NOWEAPONANI(TEXT("/SYCharacter/Sword_Elf/Animations/ABP_SY_Elf_NoWeapon1.ABP_SY_Elf_NoWeapon1_C"));
	{
		AniNoWeapon = NOWEAPONANI.Class;
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> SWORDANI(TEXT("/SYCharacter/Sword_Elf/Animations/ABP_SY_Elf_Sword.ABP_SY_Elf_Sword_C"));
	{
		AniSwordWeapon = SWORDANI.Class;
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> BOWANI(TEXT("/SYCharacter/Sword_Elf/Animations/ABP_SY_Elf_Bow.ABP_SY_Elf_Bow_C"));
	{
		AniBowWeapon = BOWANI.Class;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BOMBANI(TEXT("/SYCharacter/Sword_Elf/Animations/ABP_ElfGripThrowAction.ABP_ElfGripThrowAction_C"));		
	{
		AniBombWeapon = BOMBANI.Class;
	}




	//파티클

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> P_COMBOONE(TEXT("/Game/Particles/P_Hit2.P_Hit2"));
	//if (P_COMBOONE.Succeeded())
	//{
	//	HitParticle = P_COMBOONE.Object;

	//}


	SetControlMode(EcontrolMode::THIRDFAR);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	CurrentWeapon = WeaponList::NOWEAPON;
	DirectionToMove = FVector(0, 0, 0);
	MaxHP = 100.0f;
	CurrentHP = MaxHP;
	AttackStat = 20.0f;
	IsAttacking = false;
	IsRolling = false;
	IsShooting = false;

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	MaxSP = 100.0f;
	CurrentSP = 100.0f;
	IsBowavailable = true;
	IsBombavailable = true;

}

void APlayerCharacter::SetCharacterState(ECharacterState NewState)
{
	//ABCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		break;
	}
	case ECharacterState::READY:
	{
		break;
	}
	case ECharacterState::DEAD:
	{
		break;
	}
	}
}

ECharacterState APlayerCharacter::GetCharacterState() const
{
	return CurrentState;
}
// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


}
bool APlayerCharacter::SetSP(float Delta)
{
	if (CurrentSP + Delta < 0) return false;
	CurrentSP = FMath::Clamp<float>(CurrentSP + Delta, 0.0f, MaxSP);
	OnSPChanged.Broadcast(CurrentSP);
	return true;
}
void APlayerCharacter::SetAvailableWeapon(WeaponList availableWeapon)
{
	switch (availableWeapon)
	{
	case WeaponList::BOW:
	{
		IsBowavailable = true;
		break;
	}
	case WeaponList::BOMB:
	{
		IsBombavailable = true;
		break;
	}
	}
}
bool APlayerCharacter::IsAvailableWeapon(WeaponList Weapon)
{
	switch (Weapon)
	{
	case WeaponList::BOW:
	{
		return IsBowavailable;
		break;
	}
	case WeaponList::BOMB:
	{
		return IsBombavailable;
		break;
	}
	}
	return false;
}
// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SP채우기
	if (CurrentSP < MaxSP)
	{
		SetSP(15 * DeltaTime);
	}
	//CameraBoom->GetTargetRotation()
	
	FVector MovLoc = FMath::VInterpTo(CameraBoom->SocketOffset, DirectionToMove, DeltaTime, 3);
	CameraBoom->SocketOffset = MovLoc;
	//FVector MovLoc = FMath::VInterpTo(CameraBoom->GetRelativeTransform().GetLocation(), DirectionToMove, DeltaTime, 3);
	//CameraBoom->SetRelativeLocation(MovLoc);
	float MovArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ArmLengthTo, DeltaTime, 3);
	CameraBoom->TargetArmLength = MovArmLength;
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SwordAttackOn.AddDynamic(this, &APlayerCharacter::AttackCheck);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AniNoWeapon);
	AniCurrent = AniNoWeapon;
	auto DnEGameInstance = Cast<UDnE_GameInstance>(GetGameInstance());
	if (DnEGameInstance == nullptr)
	{
		return;
	}
	SaveSoltName = DnEGameInstance->SlotName;
	LoadData();
	ProjectileClass = ASY_Projectile::StaticClass();
	/*SwordOn.AddDynamic(this, &APlayerCharacter::SetSword);
	BombOn.AddDynamic(this, &APlayerCharacter::PutOnBomb);
	BowOn.AddDynamic(this, &APlayerCharacter::SetBow);*/
	//ShootOn.AddDynamic(this, &APlayerCharacter::Shoot);
	//OnHPIsZero.AddDynamic(this, &APlayerCharacter::SetHP);
	//OnHPChanged.AddDynamic(this, &APlayerCharacter::SetHP);
}
void APlayerCharacter::LoadData()
{
	auto DnESaveGame = Cast<UDnE_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSoltName, 0));
	if (DnESaveGame == nullptr)
	{
		DnESaveGame = GetMutableDefault<UDnE_SaveGame>();
	}
	
	//GetWorld()->SetCurrentLevel();
	SetHP(DnESaveGame->CurrentHP);
	IsBowavailable = DnESaveGame->IsBowavailable;
	IsBombavailable = DnESaveGame->IsBombavailable;
	APlayerCharacter::iPotionNum = DnESaveGame->iPotionNum;
	OnPotionNumChanged.Broadcast(APlayerCharacter::iPotionNum);
	//////////////////////////////
	///UI PART ACHIEVEMENT DATA///

	CurrentAchieved = DnESaveGame->AchievementDataMap;

	///UI PART ACHIEVEMENT DATA///
	//////////////////////////////

}
void APlayerCharacter::SavePlayerData()
{
	UDnE_SaveGame* NewPlayerData = NewObject<UDnE_SaveGame>();
	
	NewPlayerData->DneLevelName = GetWorld()->GetCurrentLevel()->GetName();
	NewPlayerData->CurrentHP = APlayerCharacter::CurrentHP;
	NewPlayerData->IsBombavailable = IsBombavailable;
	NewPlayerData->IsBowavailable = IsBowavailable;
	NewPlayerData->iPotionNum = APlayerCharacter::iPotionNum;

	//////////////////////////////
	///UI PART ACHIEVEMENT DATA///

	NewPlayerData->AchievementDataMap = CurrentAchieved;

	///UI PART ACHIEVEMENT DATA///
	//////////////////////////////

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSoltName, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("SAVE ERROR"));

	}


}

void APlayerCharacter::SetAchievement(int AchievementID)
{
	if (CurrentAchieved.Contains(AchievementID))
	{
		CurrentAchieved.Emplace(AchievementID, true);
		return;
	}
	else
	{
		return;
	}
}


float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsRolling) return 0;
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); //* CurrentCombo;
	//FVector Direction = (GetActorLocation() - DamageCauser->GetActorLocation());
	//Direction.Normalize();
	SetDamage(FinalDamage);
	return FinalDamage;

}
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}
// Called to bind functionality to input
float APlayerCharacter::GetCurrentHP()
{
	return CurrentHP;
}

float APlayerCharacter::GetMaxHP()
{
	return MaxHP;
}
float APlayerCharacter::GetCurrentSP()
{
	return CurrentSP;
}

float APlayerCharacter::GetMaxSP()
{
	return MaxSP;
}

void APlayerCharacter::SetNoWeapon()
{
	if (CurrentWeapon == WeaponList::NOWEAPON) return;
	CurrentWeapon = WeaponList::NOWEAPON;
	OnNoWeapon.Broadcast();
}
void APlayerCharacter::SetSword()
{
	if (CurrentWeapon == WeaponList::SWORD) return;
	CurrentWeapon = WeaponList::SWORD;

	

	if (GetMesh()->IsPlaying())
	{
		GetMesh()->SetAnimInstanceClass(nullptr);
	}
	GetMesh()->SetAnimInstanceClass(AniSwordWeapon);
	AniCurrent = AniSwordWeapon;
	SwordOn.Broadcast();
	FName handr(TEXT("hand_rSocket"));
	if (GetMesh()->DoesSocketExist(handr))
	{
		ST_Sword_L->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, handr);
		ST_Sword_L->WeldToImplementation(GetMesh(), handr);
	}

	FName Bow_Socket(TEXT("Bow_pelvis_Sock"));
	if (GetMesh()->DoesSocketExist(Bow_Socket))
	{
		Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Bow_Socket);
		Bow->WeldToImplementation(GetMesh(), Bow_Socket);
	}

}
void APlayerCharacter::SetBow()
{
	if (!IsBowavailable) return;
	if (CurrentWeapon == WeaponList::BOW) return;

	CurrentWeapon = WeaponList::BOW;

	if (GetMesh()->IsPlaying())
	{
		GetMesh()->SetAnimInstanceClass(nullptr);
	}

	GetMesh()->SetAnimInstanceClass(AniBowWeapon);
	AniCurrent = AniBowWeapon;
	BowOn.Broadcast();
	
	FName lSocket(TEXT("Wepon_L"));
	if (SK_Scabbard->DoesSocketExist(lSocket))
	{
		ST_Sword_L->AttachToComponent(SK_Scabbard, FAttachmentTransformRules::KeepRelativeTransform, lSocket);
	}
	FName Bow_Socket(TEXT("Bowhand_Socket"));
	if (GetMesh()->DoesSocketExist(Bow_Socket))
	{
		Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Bow_Socket);
	}

}
void APlayerCharacter::SetBomb_Implementation()
{

}
void APlayerCharacter::PutOnBomb()
{
	if (!IsBombavailable) return;
	if (CurrentWeapon != WeaponList::BOMB)
	{
		if (GetMesh()->IsPlaying())
		{
			GetMesh()->SetAnimInstanceClass(nullptr);
		}

		GetMesh()->SetAnimInstanceClass(AniBombWeapon);
		AniCurrent = AniBombWeapon;
	}
	CurrentWeapon = WeaponList::BOMB;
	BombOn.Broadcast();

	FName lSocket(TEXT("Wepon_L"));
	if (SK_Scabbard->DoesSocketExist(lSocket))
	{
		ST_Sword_L->AttachToComponent(SK_Scabbard, FAttachmentTransformRules::KeepRelativeTransform, lSocket);
	}
	FName Bow_Socket(TEXT("Bow_pelvis_Sock"));
	if (GetMesh()->DoesSocketExist(Bow_Socket))
	{
		Bow->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Bow_Socket);
		Bow->WeldToImplementation(GetMesh(), Bow_Socket);
	}
}

void APlayerCharacter::ChangeWeapon()
{
	//SavePlayerData();
	if (IsShooting) return;
	if (CurrentWeapon == WeaponList::BOMB)
	{
		return;
	}

	if (CurrentWeapon != WeaponList::SWORD)
	{
		OnWeaponChange.Broadcast();
		SetSword();
		return;
	}
	if (CurrentWeapon != WeaponList::BOW && IsBowavailable)
	{
		OnWeaponChange.Broadcast();
		SetBow();
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);


	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SwordAttack);
	PlayerInputComponent->BindAction(TEXT("Targeting"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("Targeting"), EInputEvent::IE_Released, this, &APlayerCharacter::ZoomOut);
	PlayerInputComponent->BindAction(TEXT("Roll"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Roll);
	PlayerInputComponent->BindAction(TEXT("GainBomb"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SetBomb);
	PlayerInputComponent->BindAction(TEXT("WeaponChange"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ChangeWeapon);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interaction);
	PlayerInputComponent->BindAction(TEXT("UsePotion"), EInputEvent::IE_Pressed, this, &APlayerCharacter::UsePotion);

}
void APlayerCharacter::Jump()
{
	if (IsRolling) return;
	ACharacter::Jump();
}
void APlayerCharacter::Interaction()
{
	InteractionOn.Broadcast();
}
void APlayerCharacter::UpDown(float NewAxisValue)
{
	
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
	

	AddMovementInput(FRotator(0.f,GetControlRotation().Yaw,0.f).Vector(), NewAxisValue);
}
void APlayerCharacter::LeftRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

}
void APlayerCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}
void APlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void APlayerCharacter::SwordAttack()
{
	if (CurrentWeapon != WeaponList::SWORD)
	{
		Shoot();
		return;
	}
	SwordAttackOn.Broadcast();
	//if (!IsAttacking)
	//{
	//	SwordAttackOn.Broadcast();
	//	//IsAttacking = true;
	//}
}

void APlayerCharacter::ZoomIn()
{
	if (CurrentWeapon == WeaponList::BOW)
	{
		SetControlMode(EcontrolMode::THIRDNEAR);

	}
}
void APlayerCharacter::UsePotion()
{
	if (iPotionNum <= 0) return;
	SetPotionNum(-1);
	SetHeal(+10);
	OnCuredDel.Broadcast();
}
void APlayerCharacter::SetPotionNum(int Delta)
{
	iPotionNum += Delta;
	OnPotionNumChanged.Broadcast(iPotionNum);
}
void APlayerCharacter::ZoomOut()
{
	SetControlMode(EcontrolMode::THIRDFAR);
}
void APlayerCharacter::Shoot()
{
	if (CurrentWeapon != WeaponList::BOMB && CurrentWeapon != WeaponList::BOW)
	{
		return;
	}


	if (CurrentWeapon == WeaponList::BOMB)
	{
		BombOff.Broadcast();
		return;
	}
	if (IsShooting) return;
	if (ProjectileClass != NULL)
	{
		if (GetWorld() != NULL)
		{
			//FVector CameraLocation;
			//FRotator CameraRotation;
			//GetActorEyesViewPoint(CameraLocation, CameraRotation);
			//FVector MuzzleOffset;
			//// Transform MuzzleOffset from camera space to world space.
			//FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
			//FRotator MuzzleRotation = CameraRotation;
			//// Skew the aim to be slightly upwards.
			//MuzzleRotation.Pitch += 10.0f;
			//UWorld* World = GetWorld();
			//if (World)
			//{
			//	FActorSpawnParameters SpawnParams;
			//	SpawnParams.Owner = this;
			//	SpawnParams.Instigator = Instigator;
			//	// Spawn the projectile at the muzzle.
			//	ASY_Projectile* Projectile = World->SpawnActor<ASY_Projectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			//	if (Projectile)
			//	{
			//		// Set the projectile's initial trajectory.
			//		FVector LaunchDirection = MuzzleRotation.Vector();
			//		Projectile->FireInDirection(LaunchDirection);
			//	}
			//}
			
			ShootOn.Broadcast();
			FVector SocketLoc;
			SocketLoc = Bow->GetSocketLocation("BowEmitterSocket");


			//const FRotator SpawnRotation = Controller->GetControlRotation();
			//GetMesh()->GetForwardVector();
			// const FRotator SpawnRotation = Bow->GetSocketRotation("BowEmitterSocket");
			FRotator SpawnRotation = Bow->GetSocketRotation("BowEmitterSocket");

			/*APlayerCameraManager::GetActorRotation();
			FRotator CameraRotation = APlayerCameraManager::GetActorRotation();
			SpawnRotation.Yaw = CameraRotation.Yaw;*/

			
			
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//
			FHitResult Hiter;
			FVector Start = FollowCamera->K2_GetComponentLocation();
			float Tracedistance = 30000;
			FVector End = Start + (FollowCamera->GetForwardVector()*Tracedistance);
			
			FCollisionQueryParams Param;
			Param.bTraceComplex = true;
			//Start = GetActorLocation();
			
			/*DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);
			*/
			//bool bHit = ActorLineTraceSingle(Hiter, Start, End, ECollisionChannel::ECC_WorldStatic, Param);
			bool bHit = GetWorld()->LineTraceSingleByChannel(Hiter, Start, End, ECollisionChannel::ECC_Visibility, Param);
			if (bHit)
			{
				

				
				GetWorld()->SpawnActor<AActor>(ProjectileClass, SocketLoc, (Hiter.Location - SocketLoc).Rotation(), ActorSpawnParams);

			}
			else
			{
				//기존
				GetWorld()->SpawnActor<AActor>(ProjectileClass, SocketLoc, (End - SocketLoc).Rotation(), ActorSpawnParams);
			}
			//
		
			
		}
	}

}
float APlayerCharacter::GetAttackStat()
{
	return AttackStat;
}
void APlayerCharacter::Roll()
{
	if (!IsRolling)
	{
		if (!SetSP(-50))
		{
			return;
		}
		
		RollOn.Broadcast();
	}
}

void APlayerCharacter::AttackCheck()
{
//	TArray<FHitResult> HitResults;
//	FCollisionQueryParams Params(NAME_None, false, this);
//
//
//	ST_Sword_L->BeginComponentOverlap()
//	bool bResult = GetWorld()->SweepMultiByProfile(
//		HitResults,
//		GetActorLocation(),
//		GetActorLocation() + GetActorForwardVector() * AttackRange,
//		FQuat::Identity,
//		TEXT("PlayerAttack"),
//		FCollisionShape::MakeSphere(AttackRadius),
//		Params);
////#if ENABLE_DRAW_DEBUG
////	FVector TraceVec = GetActorForwardVector() * AttackRange;
////	FVector Center = GetActorLocation() + TraceVec * 0.5f;
////	float HalfHeight = AttackRange * 0.5f + AttackRadius;
////	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
////	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
////	float DebugLifeTime = 5.0f;
////
////	DrawDebugCapsule(
////		GetWorld(),
////		Center,
////		HalfHeight,
////		AttackRadius,
////		CapsuleRot,
////		DrawColor,
////		false,
////		DebugLifeTime);
////
////#endif
//	if (bResult)
//	{
//		for (auto& HitResult : HitResults)
//		{
//			if (HitResult.Actor.IsValid())
//			{
//				UE_LOG(LogTemp, Warning, TEXT("Throw Damage Actor Name : %s"), *HitResult.Actor->GetName());
//				FDamageEvent DamageEvent;
//				float CurrentDamage = 50;
//				HitResult.Actor->TakeDamage(CurrentDamage, DamageEvent, GetController(), this);
//				HitResult.GetFirstBlockingHit(HitResults);
//				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.GetFirstBlockingHit(HitResults)->Location);
//				// ->Location;
//				/*되는거
//				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location);
//				*/
//			}
//		}
//
//
//	}
}
void APlayerCharacter::SetControlMode(EcontrolMode NewControlMode)
{
	switch (NewControlMode)
	{
	case EcontrolMode::THIRDFAR:
	{
		ArmLengthTo = 600.0f;
		DirectionToMove = FVector(0.0f, 0.0f, 0.0f);
		CurrentControlMode = EcontrolMode::THIRDFAR;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CameraBoom->bUsePawnControlRotation = true;
		ZoomOutDel.Broadcast();
		break;
	}

	case EcontrolMode::THIRDNEAR:
	{
		CurrentControlMode = EcontrolMode::THIRDNEAR;
		ArmLengthTo = 100.0f;
		DirectionToMove = FVector(0.0f, 54.0f, 82.0f);
		//DirectionToMove = FVector(524.0f, 44.0f, 101.0f);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		ZoomInDel.Broadcast();
	}
	}
}

void APlayerCharacter::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP));

}
void APlayerCharacter::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast(NewHP);

	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast(NewHP);
		IsDead = true;
		/*if (GetMesh()->IsPlaying())
		{
			GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->SetAnimInstanceClass(nullptr);
		}
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		GetMesh()->GetAnimInstance()->Montage_Play(MontageDead, 1, EMontagePlayReturnType::MontageLength, 0.5f, true);*/
		SetActorEnableCollision(false);
	}
}
void APlayerCharacter::SetHeal(float NewHeal)
{
	SetHP(FMath::Clamp<float>(CurrentHP + NewHeal, CurrentHP, MaxHP));
}