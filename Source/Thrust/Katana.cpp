// Fill out your copyright notice in the Description page of Project Settings.


#include "Katana.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"


void AKatana::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		// 플레이어가 조종하는 Pawn 가져오기
		APawn* PlayerPawn = PlayerController->GetPawn();

		// Pawn이 실제로 ACharacter인지 확인하고 캐스팅
		ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerPawn);

		if (PlayerCharacter)
		{
			PlayerCharacter->JumpMaxCount = 2;
		}
	}
}

AKatana::AKatana()
{
	DashMaxSpeed = 2600;
	WalkSpeed = 650;
	Delay = 60;

	PrimaryActorTick.bCanEverTick = true;

	WeaponRoot->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Weapon/MeleeWeapon/KatanaHand.KatanaHand")));
	UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBody"));
	StaticMeshComponent->SetupAttachment(WeaponRoot);
	StaticMeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Weapon/MeleeWeapon/KatanaBody.KatanaBody")));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorScale3D(FVector(0.5, 0.5, 0.5));
}
void AKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AKatana::Skill(FVector Hit)
{
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//if (PlayerController)
	//{
	//	ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());

	//	if (PlayerCharacter)
	//	{
	//		// 새로운 위치로 이동
	//		PlayerCharacter->SetActorLocation(Hit);
	//	}
	//}
	return true;
}

void AKatana::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("katana attack"));
	bCanAttack = false;
}
