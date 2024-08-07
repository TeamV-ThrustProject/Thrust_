// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Rifle.h"
#include "Katana.h"
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

	UPROPERTY()
	float GravityScale = 3.0f;
	UPROPERTY()
	float JumpForce = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponBase>MainWeapon;

	AWeaponBase* mw;

	UPROPERTY()
	bool bKatana;

	UPROPERTY()
	bool bDash=false;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void StartDash();

	UFUNCTION()
	void StopDash();

	UFUNCTION()
	void StartAttack();

	UFUNCTION()
	void StopAttack();

	UFUNCTION()
	void UseSkill();

	UFUNCTION(BlueprintCallable)
	void CastMainWeapon();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	ARifle* Rifle;

	UPROPERTY(EditAnywhere)
	AKatana* Katana;

	UPROPERTY()
	bool bAttack;

	FVector h;
};
