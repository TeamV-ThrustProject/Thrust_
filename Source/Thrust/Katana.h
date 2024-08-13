// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Katana.generated.h"

/**
 * 
 */
UCLASS()
class THRUST_API AKatana : public AWeaponBase
{
	GENERATED_BODY()
public:
	AKatana();	
	virtual void Tick(float DeltaTime) override;
	virtual bool Skill(FVector Hit) override;
	virtual void Attack()override;
};