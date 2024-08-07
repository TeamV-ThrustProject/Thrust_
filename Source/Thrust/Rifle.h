// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class THRUST_API ARifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARifle();
	int ReloadTime=60;
	bool bReload;

	void Tick(float DeltaTime)override;
	virtual void Attack()override;
};
