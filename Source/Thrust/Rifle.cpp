// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "Bullet.h"
#include "Katana.h"

ARifle::ARifle()
{
	DashMaxSpeed = 1200;
	WalkSpeed = 650;
	Delay = 6;
	MaxBullet = 33;
	Bullet = 33;

	WeaponRoot->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Weapon/Gun/GunBody.GunBody")));
	for(int i=1;i<11;i++)
	{
		FString MeshName = FString::Printf(TEXT("Body%d"), i);
		FString MeshPath = FString::Printf(TEXT("/Game/Weapon/Gun/Gun%d.Gun%d"), i, i);
		UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*MeshName);
		StaticMeshComponent->SetupAttachment(WeaponRoot);
		StaticMeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *MeshPath));
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	BulletPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletPoint"));
	BulletPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletPoint->SetRelativeLocation(FVector(-142, 27, 54));

}

void ARifle::Attack()
{
	if (bReload)
		return;

	UE_LOG(LogTemp, Warning, TEXT("rifle attack"));

	FVector SpawnLocation = FVector(8824.f, -400.f, 870.f);
	FRotator SpawnRotation = FRotator(0, 0, 180);


	//GetWorld()->SpawnActor<ARifle>(ARifle::StaticClass(), SpawnLocation, SpawnRotation);
	GetWorld()->SpawnActor<AWeaponBase>(AKatana::StaticClass());
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
