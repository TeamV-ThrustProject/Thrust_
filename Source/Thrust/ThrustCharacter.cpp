// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrustCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"



// Sets default values
AThrustCharacter::AThrustCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    MovementComponent = GetCharacterMovement();

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("SkeletalMesh'/Game/Human/HumanTest2.HumanTest2'"));
    if (SkeletalMeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClassFinder(TEXT("/Script/Engine.AnimBlueprint'/Game/Siru/BP/ABP_Character.ABP_Character_C'"));
    if (AnimBPClassFinder.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(AnimBPClassFinder.Class);
    }

    GetMesh()->SetRelativeScale3D(FVector(3.745f, 3.745f, 3.745f));
    GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
    GetMesh()->SetRelativeLocation(FVector(0, 0, -280));

    GetCapsuleComponent()->SetCapsuleSize(55, 135);

    springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    springArmComponent->SetupAttachment(RootComponent);
    springArmComponent->SetRelativeLocation(FVector(0, 0, 0));
    springArmComponent->TargetArmLength = 0;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(springArmComponent);
    CameraComponent->SetRelativeLocation(FVector(39.4f, 0.0f, 110.0f));

    CameraComponent->bUsePawnControlRotation = true;

    MainWeapon = AKatana::StaticClass();    
    WeaponClasses.Add(AKatana::StaticClass());
    WeaponClasses.Add(ARifle::StaticClass());

    
}

// Called when the game starts or when spawned
void AThrustCharacter::BeginPlay()
{
	Super::BeginPlay();
    CastMainWeapon();
    for (TSubclassOf<AWeaponBase> WeaponClass : WeaponClasses)
    {
        if (WeaponClass)
        {
            // WeaponClass에 해당하는 무기 인스턴스를 스폰
            AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

            if (SpawnedWeapon)
            {
                // 스폰된 무기를 배열에 추가
                WeaponArray.Add(SpawnedWeapon);

                // 예시로 무기를 바로 공격하게 함
                SpawnedWeapon->Attack();
            }
        }
    }
}

// Called every frame
void AThrustCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    mw->Tick(DeltaTime);
    if(mw != nullptr)
        if (bDash)
        {
            if (GetCharacterMovement()->MaxWalkSpeed < mw->DashMaxSpeed)
                GetCharacterMovement()->MaxWalkSpeed += (mw->DashMaxSpeed - mw->WalkSpeed)/120;
        }
        else
        {
            if (GetCharacterMovement()->MaxWalkSpeed > mw->WalkSpeed)
                GetCharacterMovement()->MaxWalkSpeed -= (mw->DashMaxSpeed - mw->WalkSpeed) / 60;
        }
    else 
        UE_LOG(LogTemp, Warning, TEXT("cast error"));

    if(bAttack)
    if (mw != nullptr)
    {
        if (mw->bCanAttack)
        {
            mw->Attack();
        }
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        FVector CameraLocation;
        FRotator CameraRotation;
        PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

        FHitResult HitResult;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, CameraLocation + CameraRotation.Vector() * 10000.0f, ECC_Visibility))
        {
            h = HitResult.ImpactPoint;
        }
    }

}

// Called to bind functionality to input
void AThrustCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
    {
        Super::SetupPlayerInputComponent(PlayerInputComponent);
         
        GetCharacterMovement()->JumpZVelocity = JumpForce;
        GetCharacterMovement()->GravityScale = GravityScale;
        
        PlayerInputComponent->BindAxis("MoveForward", this, &AThrustCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &AThrustCharacter::MoveRight);
         
        PlayerInputComponent->BindAxis("Turn", this, &AThrustCharacter::AddControllerYawInput);
        PlayerInputComponent->BindAxis("LookUp", this, &AThrustCharacter::AddControllerPitchInput);

        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AThrustCharacter::StartJump);
        PlayerInputComponent->BindAction("Jump", IE_Released, this, &AThrustCharacter::StopJump);

        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AThrustCharacter::Run);

        PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AThrustCharacter::StartAttack);
        PlayerInputComponent->BindAction("Attack", IE_Released, this, &AThrustCharacter::StopAttack);

        PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &AThrustCharacter::UseSkill);

        PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AThrustCharacter::Dash);

}

void AThrustCharacter::MoveForward(float Value)
{
    if (Value != 0.0f) // 입력 값이 0이 아닐 때만 이동
    {
        // 카메라의 방향을 가져옴
        FRotator ControlRotation = Controller->GetControlRotation();

        // 카메라의 Yaw 값만 사용하여 방향을 결정
        ControlRotation.Pitch = 0; // Pitch는 0으로 설정하여 수평으로만 이동
        ControlRotation.Roll = 0; // Roll은 무시

        // 방향 벡터 계산
        FVector Direction = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);

        // 이동 입력 적용
        AddMovementInput(Direction, Value);
    }
}

void AThrustCharacter::MoveRight(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AThrustCharacter::StartJump()
{
    bPressedJump = true;
}

void AThrustCharacter::StopJump()
{
    bPressedJump = false;
}

void AThrustCharacter::Run()
{
    bDash = !bDash;
}

void AThrustCharacter::Dash()
{
    FVector Dash = this->GetCharacterMovement()->GetLastInputVector();
    if (MovementComponent->IsFalling())
    {
        Dash *= 2000;
    }
    else
    {
        Dash *= 4000;
    }
    if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
    {
        Dash += FVector(0, 0, 200);
        LaunchCharacter(Dash, true, true);
    }
}

void AThrustCharacter::StartAttack()
{
    bAttack = true;
}

void AThrustCharacter::StopAttack()
{
    bAttack = false;
}

void AThrustCharacter::UseSkill()
{
    if (mw != nullptr)
    {
        if (mw->Skill(h))
        {
            this->SetActorLocation(h);
        }
    }
}

void AThrustCharacter::CastMainWeapon()
{
    mw = Cast<AWeaponBase>(MainWeapon->ClassDefaultObject);

    AWeaponBase* SpawnActor = GetWorld()->SpawnActor<AWeaponBase>(MainWeapon, GetActorTransform());

 /*   SpawnActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "Weapon");
    SpawnActor->SetActorRelativeScale3D(FVector(0.16f, 0.16f, 0.16f));
    SpawnActor->SetActorRelativeLocation(FVector(0.12f, 2.2f, -5.2f));
    SpawnActor->SetActorRelativeRotation(FRotator(0, 180, 0));*/
}


