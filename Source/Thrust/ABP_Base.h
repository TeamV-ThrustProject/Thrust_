// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_Base.generated.h"

/**
 * 
 */
UCLASS()
class THRUST_API UABP_Base : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int WeaponNum = -1;
};
