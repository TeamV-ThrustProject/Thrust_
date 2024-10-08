// Fill out your copyright notice in the Description page of Project Settings.


#include "Save.h"
#include "../Player/ThrustCharacter.h"
#include "Components/BoxComponent.h"

ASave::ASave()
{
	//PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionProfileName("Trigger");	//충돌을 트리거로 설정
	CollisionBox->SetGenerateOverlapEvents(true);	//오버랩 이벤트 활성화
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));	//**충돌 채널 설정**

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASave::OnOverlapBegin);
}

void ASave::BeginPlay()
{
	Super::BeginPlay();
}

void ASave::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	AThrustCharacter* Player = Cast<AThrustCharacter>(OtherActor);
	if (Player)
	{
		Player->LastSavedTransform.SetLocation(this->GetActorTransform().GetLocation());
	}
}

