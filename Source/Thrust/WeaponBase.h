// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AThrustCharacter;

UCLASS()
class THRUST_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	UStaticMeshComponent* WeaponRoot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() {};
	virtual bool Skill(FVector hit) { return false; };

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
	int SkillCooldown;
	UPROPERTY()
	int Delay;
	UPROPERTY()
	int t;

	UPROPERTY()
	bool bCanAttack = true;

};
