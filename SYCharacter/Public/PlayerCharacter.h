// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SYCharacter.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwordAttackOnDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwordONDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBowONDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangeWeaponDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBombONDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBombOffDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoWeaponOnDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRollOnDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetingDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoomInDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoomOutDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCuredDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHPIsZeroDelegate, float, SomeParameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float, SomeParameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSPChangedDelegate, float, SomeParameter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPotionNumChangedOnDelegate, int, SomeParameter);
UENUM(BlueprintType)
enum class WeaponList :uint8
{
	NOWEAPON,
	SWORD,
	BOW,
	BOMB
};
UENUM(BlueprintType)
enum class EcontrolMode :uint8
{
	THIRDNEAR,
	THIRDFAR
};
UCLASS()
class SYCHARACTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

	FString SaveSoltName;
	UFUNCTION(BlueprintCallable, Category = Save)
	void SavePlayerData();
	UFUNCTION(BlueprintCallable, Category = Save)
	void LoadData();

	//////////////////////////////
	///UI PART ACHIEVEMENT DATA///

	UPROPERTY(BlueprintReadWrite, Category = AchievementData)
	TMap<int, bool> CurrentAchieved;

	UFUNCTION(BlueprintCallable)
		void SetAchievement(int AchievementID);

	///UI PART ACHIEVEMENT DATA///
	//////////////////////////////



private:
	TSubclassOf<UAnimInstance> AniNoWeapon;
	TSubclassOf<UAnimInstance> AniSwordWeapon;
	TSubclassOf<UAnimInstance> AniBowWeapon;
	TSubclassOf<UAnimInstance> AniBombWeapon;
	TSubclassOf<UAnimInstance> AniCurrent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* StunMontage;
private:
	bool IsBowavailable;
	bool IsBombavailable;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
		bool IsDead = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
		float ComboBonus = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
		int iPotionNum = 0;
	UFUNCTION(BlueprintCallable)
		void SetPotionNum(int Delta);
	UFUNCTION(BlueprintCallable)
		void UsePotion();
	UFUNCTION(BlueprintCallable)
		float GetAttackStat();
private:
	float MaxHP=100.0f;
	float CurrentHP=100.0f;
	float AttackStat=20.0f;

	float MaxSP = 100.0f;
	float CurrentSP = 100.0f;
public:
	void SetDamage(float NewDamage);
	void SetHeal(float NewHeal);
	bool SetSP(float Delta);

public:
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetAvailableWeapon(WeaponList availableWeapon);
	UFUNCTION(BlueprintCallable, Category = Weapon)
		bool IsAvailableWeapon(WeaponList availableWeapon);
	UFUNCTION(BlueprintCallable, Category = Stat)
		float GetCurrentHP();
	UFUNCTION(BlueprintCallable, Category = Stat)
		float GetMaxHP();
	UFUNCTION(BlueprintCallable, Category = Stat)
		float GetCurrentSP();
	UFUNCTION(BlueprintCallable, Category = Stat)
		float GetMaxSP();


	UFUNCTION(BlueprintCallable, Category = Camera)
		void SetControlMode(EcontrolMode NewControlMode);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	EcontrolMode CurrentControlMode = EcontrolMode::THIRDFAR;
	FVector DirectionToMove = FVector::ZeroVector;

	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmZLocationTo = 0.0f;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;


	// ÆÄÆ¼Å¬
public:
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		WeaponList CurrentWeapon = WeaponList::NOWEAPON;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* FollowCamera;

	FVector MuzzleOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		UPointLightComponent* PointLightFront;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		UPointLightComponent* PointLightBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMeshComponent* SK_Scabbard;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMeshComponent* Bow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		UStaticMeshComponent* ST_Sword_S;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		UStaticMeshComponent* ST_Sword_L;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class AActor> ProjectileClass;

public:
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetSword();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetNoWeapon();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetBow();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void PutOnBomb();
	UFUNCTION(BlueprintNativeEvent, Category = Weapon)
		void SetBomb();
		virtual void SetBomb_Implementation();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void ChangeWeapon();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void Shoot();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void Roll();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void ZoomIn();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void ZoomOut();
	UFUNCTION(BlueprintCallable, Category = Weapon)
		void SetHP(float NewHP);
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Jump();
	void Turn(float NewAxisValue);
	void Interaction();


	//°ø°Ý
	UFUNCTION()
		void SwordAttack();


private:
	UFUNCTION()
		void AttackCheck();
	//Ä¸½¶
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;
public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsBombing;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsShooting;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Move, Meta = (AllowPrivateAccess = true))
		bool IsRolling;
	UPROPERTY(BlueprintAssignable)
		FSwordAttackOnDelegate  SwordAttackOn;
	UPROPERTY(BlueprintAssignable)
		FBowONDelegate BowOn;
	UPROPERTY(BlueprintAssignable)
		FSwordONDelegate SwordOn;
	UPROPERTY(BlueprintAssignable)
		FBombONDelegate BombOn;
	UPROPERTY(BlueprintAssignable)
		FBombOffDelegate BombOff;
	UPROPERTY(BlueprintAssignable)
		FRollOnDelegate RollOn;
	UPROPERTY(BlueprintAssignable)
		FTargetingDelegate TargetOn;
	UPROPERTY(BlueprintAssignable)
		FShootDelegate ShootOn;
	UPROPERTY(BlueprintAssignable)
		FOnHPIsZeroDelegate OnHPIsZero;
	UPROPERTY(BlueprintAssignable)
		FOnHPChangedDelegate  OnHPChanged;
	UPROPERTY(BlueprintAssignable)
		FNoWeaponOnDelegate OnNoWeapon;
	UPROPERTY(BlueprintAssignable)
		FOnSPChangedDelegate OnSPChanged;
	UPROPERTY(BlueprintAssignable)
		FChangeWeaponDelegate OnWeaponChange;
	UPROPERTY(BlueprintAssignable)
		FInteractDelegate InteractionOn;
	UPROPERTY(BlueprintAssignable)
		FZoomInDelegate	ZoomInDel;
	UPROPERTY(BlueprintAssignable)
		FZoomOutDelegate ZoomOutDel;
	UPROPERTY(BlueprintAssignable)
		FPotionNumChangedOnDelegate OnPotionNumChanged;
	UPROPERTY(BlueprintAssignable)
		FCuredDelegate OnCuredDel;
	private:
		UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
			ECharacterState CurrentState;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


};
