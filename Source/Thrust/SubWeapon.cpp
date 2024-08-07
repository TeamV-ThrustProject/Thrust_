// Fill out your copyright notice in the Description page of Project Settings.


#include "SubWeapon.h"

// Sets default values
ASubWeapon::ASubWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASubWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

