// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

ARifle::ARifle()
{
	DashMaxSpeed = 1300;
	WalkSpeed = 650;
	Delay = 6;
	MaxBullet = 33;
	Bullet = 33;

	WeaponRoot->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Siru/Gun/GunBody.GunBody")));
	for(int i=1;i<11;i++)
	{
		FString MeshName = FString::Printf(TEXT("Body%d"), i);
		FString MeshPath = FString::Printf(TEXT("/Game/Siru/Gun/Gun%d.Gun%d"), i, i);
		UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*MeshName);
		StaticMeshComponent->SetupAttachment(WeaponRoot);
		StaticMeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *MeshPath));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ARifle::Attack()
{
	if (bReload)
		return;

	UE_LOG(LogTemp, Warning, TEXT("rifle attack"));
	Bullet--;
	bCanAttack = false;

	if (Bullet < 1)
		bReload = true;
}

void ARifle::Tick(float DeltaTime)
{
	if (bReload)
	{
		t++;
		UE_LOG(LogTemp, Warning, TEXT("Reloading"));

		if (ReloadTime < t)
		{
			Bullet = 33;
			bReload = false;
			bCanAttack = true;
			t = 0;
			UE_LOG(LogTemp, Warning, TEXT("Ready"));
		}
		return;
	}

	Super::Tick(DeltaTime);
}
