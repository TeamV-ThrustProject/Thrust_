// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWeapon.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMainWeapon::AMainWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bCanAttack = true;
	PrimaryActorTick.bCanEverTick = true;
	WeaponRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponRoot"));
	RootComponent = WeaponRoot;
}

// Called when the game starts or when spawned
void AMainWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AMainWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bCanAttack)
	{
		t++;
		if (Delay < t)
		{
			t = 0;
			bCanAttack = true;
		}
		return;
	}
}

