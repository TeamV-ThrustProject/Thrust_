// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainWeapon.generated.h"

UCLASS()
class THRUST_API AMainWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMeshComponent* WeaponRoot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() {};

	UPROPERTY()
	float DashMaxSpeed;
	UPROPERTY()
	float WalkSpeed;
	UPROPERTY()
	float WeaponDamage;
	UPROPERTY()
	int MaxBullet;
	UPROPERTY()
	int Bullet;

	UPROPERTY()
	int Delay;
	UPROPERTY()
	int t;

	UPROPERTY()
	bool bCanAttack=true;
};
