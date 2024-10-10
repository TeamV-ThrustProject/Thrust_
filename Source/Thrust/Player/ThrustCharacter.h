// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Weapon/Rifle.h"
#include "../Weapon/Katana.h"
#include "../Anim/ABP_Base.h"
#include "ThrustCharacter.generated.h"


UCLASS()
class THRUST_API AThrustCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThrustCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InComingDamage = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform LastSavedTransform;

	UPROPERTY()
	float GravityScale = 3.0f;
	UPROPERTY()
	float JumpForce = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponBase>MainWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AWeaponBase>> WeaponClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AWeaponBase*> WeaponArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WeaponNum = 0;

	UPROPERTY()
	UABP_Base *AnimInstance;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	AWeaponBase* mw;

	UPROPERTY()
	bool bKatana;

	UPROPERTY()
	bool isSlide = false;

	UPROPERTY()
	bool isDash = true;

	UPROPERTY()
	int DashCoolTime = 500;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartSlide();

	UFUNCTION()
	void EndSlide();

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	void StopAttack();

	UFUNCTION()
	void UseSkill();

	UFUNCTION(BlueprintCallable)
	void CastMainWeapon();

	UFUNCTION()
	void SwapWeapon();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComponent;

	UPROPERTY(EditAnywhere)
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	ARifle* Rifle;

	UPROPERTY(EditAnywhere)
	AKatana* Katana;

	UPROPERTY()
	bool bAttack;

	UPROPERTY()
	UABP_Base *AnimBP;

	FVector h;
};
