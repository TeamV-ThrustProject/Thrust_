// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

ARifle::ARifle()
{
	DashMaxSpeed = 1200;
	WalkSpeed = 650;
	Delay = 30;
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
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
	BulletSpawnLocation = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass());
	BulletSpawnLocation->Speed = 0;
	BulletSpawnLocation->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	BulletSpawnLocation->SetActorRelativeLocation(FVector(-140, 27, 55));
	BulletSpawnLocation->SetActorHiddenInGame(true);
	BulletSpawnLocation->SetActorEnableCollision(false);
	BulletSpawnLocation->SetActorTickEnabled(false);

}

void ARifle::Attack(FVector hit)
{
	if (bReload)
		return;

	UE_LOG(LogTemp, Warning, TEXT("rifle attack"));

	AActor *bullet = GetWorld()->SpawnActor<AActor>(ABullet::StaticClass());
	bullet->SetActorLocation(BulletSpawnLocation->GetActorLocation());

	bullet->SetActorRotation(FRotationMatrix::MakeFromX((hit - BulletSpawnLocation->GetActorLocation()).GetSafeNormal()).Rotator());
	
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
