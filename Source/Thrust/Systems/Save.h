// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Save.generated.h"

UCLASS()
class THRUST_API ASave : public AActor
{
	GENERATED_BODY()
	
public:	
	ASave();
	//virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* CollisionBox;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
