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

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClassFinder(TEXT("/Script/Engine.AnimBlueprint'/Game/Siru/BP/Anim/ABP_CharacterTest.ABP_CharacterTest_C'"));
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
    springArmComponent->TargetArmLength = 300.0f;
    springArmComponent->bUsePawnControlRotation = true;
    springArmComponent->bDoCollisionTest = true;
    springArmComponent->ProbeSize = 12.0f; // 충돌을 감지하는 구의 크기
    springArmComponent->ProbeChannel = ECC_Camera; // 카메라 충돌 채널 설정
    springArmComponent->bEnableCameraLag = true;
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(springArmComponent);
    CameraComponent->SetRelativeLocation(FVector(39.4f, 90.0f, 110.0f));

    CameraComponent->bUsePawnControlRotation = false;

    //MainWeapon = AKatana::StaticClass();    
    WeaponClasses.Add(ARifle::StaticClass());
    WeaponClasses.Add(AKatana::StaticClass());

}

// Called when the game starts or when spawned
void AThrustCharacter::BeginPlay()
{
	Super::BeginPlay();
    for (TSubclassOf<AWeaponBase> WeaponClass : WeaponClasses)
    {
        if (WeaponClass)
        {
            // WeaponClass�� �ش��ϴ� ���� �ν��Ͻ��� ����
            AWeaponBase* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

            if (SpawnedWeapon)
            {
                WeaponArray.Add(SpawnedWeapon);
                SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "Weapon");
                SpawnedWeapon->SetActorRelativeScale3D(FVector(0.16f, 0.16f, 0.16f));
                SpawnedWeapon->SetActorRelativeLocation(FVector(0.12f, 2.2f, -5.2f));
                SpawnedWeapon->SetActorRelativeRotation(FRotator(-22.3, 214.7, 12.9));

                SpawnedWeapon->SetActorHiddenInGame(true);
                SpawnedWeapon->SetActorEnableCollision(false);
                SpawnedWeapon->SetActorTickEnabled(false);
            }
        }
    }
    SwapWeapon();
    CastMainWeapon();

}

// Called every frame
void AThrustCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    mw->Tick(DeltaTime);
    if (mw != nullptr)
    {
        if (GetCharacterMovement()->MaxWalkSpeed < mw->DashMaxSpeed)
            GetCharacterMovement()->MaxWalkSpeed += (mw->DashMaxSpeed - mw->WalkSpeed) / 120;
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("MainWeapon is Null"));

    if (bAttack&&!isSlide)
        if (mw != nullptr)
        {
            if (mw->bCanAttack)
            {
                mw->Attack(h);
            }
        }

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
    else
        PlayerController = Cast<APlayerController>(GetController());

    if (!isDash)
    {
        DashCoolTime--;
        if (DashCoolTime == 0)
            isDash = true;
    }

    if (isSlide)
        if (GetCharacterMovement()->Velocity.IsZero())
        {
            EndSlide();
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

        PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AThrustCharacter::StartSlide);
        PlayerInputComponent->BindAction("Run", IE_Released, this, &AThrustCharacter::EndSlide);

        PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AThrustCharacter::StartAttack);
        PlayerInputComponent->BindAction("Attack", IE_Released, this, &AThrustCharacter::StopAttack);

        PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &AThrustCharacter::UseSkill);

        PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AThrustCharacter::Dash);

}

void AThrustCharacter::MoveForward(float Value)
{
    if (!isSlide)
        if (Value != 0.0f) // �Է� ���� 0�� �ƴ� ���� �̵�
        {
            FRotator ControlRotation = Controller->GetControlRotation();

            ControlRotation.Pitch = 0; // Pitch�� 0���� �����Ͽ� �������θ� �̵�
            ControlRotation.Roll = 0; // Roll�� ����

            FVector Direction = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);

            AddMovementInput(Direction, Value);
        }
}

void AThrustCharacter::MoveRight(float Value)
{
    if (!isSlide)
    {
        FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AThrustCharacter::StartJump()
{
    bPressedJump = true;
    if (isSlide) 
    {
        EndSlide();
    }
}

void AThrustCharacter::StopJump()
{
    bPressedJump = false;
}

void AThrustCharacter::StartSlide()
{
    this->isSlide = true;
    AnimBP->isSlide = true;
    MovementComponent->BrakingFrictionFactor = 0.15f;
}

void AThrustCharacter::EndSlide()
{
    this->isSlide = false;
    AnimBP->isSlide = false;
    MovementComponent->BrakingFrictionFactor = 8;
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
    if (isDash)
        if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
        {
            Dash += FVector(0, 0, 200);
            LaunchCharacter(Dash, true, true);
            isDash = false;
            DashCoolTime = 500;
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
            //this->SetActorLocation(h);
        }
    }
    SwapWeapon();
}

void AThrustCharacter::CastMainWeapon()
{
    mw = Cast<AWeaponBase>(WeaponArray[WeaponNum]);
   // AWeaponBase* SpawnActor = GetWorld()->SpawnActor<AWeaponBase>(MainWeapon, GetActorTransform());

 /*   SpawnActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "Weapon");
    SpawnActor->SetActorRelativeScale3D(FVector(0.16f, 0.16f, 0.16f));
    SpawnActor->SetActorRelativeLocation(FVector(0.12f, 2.2f, -5.2f));
    SpawnActor->SetActorRelativeRotation(FRotator(0, 180, 0));*/
}

void AThrustCharacter::SwapWeapon()
{
    if (WeaponArray[WeaponNum] != nullptr)
    {
        WeaponArray[WeaponNum]->SetActorHiddenInGame(true);
        WeaponArray[WeaponNum]->SetActorEnableCollision(false);
        WeaponArray[WeaponNum]->SetActorTickEnabled(false);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("weapon nullptr"));
   

    if (WeaponNum % 2 == 0)
        WeaponNum++;
    else
        WeaponNum--;
    CastMainWeapon();

    if (WeaponArray[WeaponNum] != nullptr)
    {
        WeaponArray[WeaponNum]->SetActorHiddenInGame(false);
        WeaponArray[WeaponNum]->SetActorEnableCollision(true);
        WeaponArray[WeaponNum]->SetActorTickEnabled(true);
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("weapon nullptr"));


    if(AnimBP !=nullptr)
        AnimBP->WeaponNum = WeaponNum;
    else
        AnimBP = Cast<UABP_Base>(GetMesh()->GetAnimInstance());

    UE_LOG(LogTemp, Warning, TEXT("weapon num %d"),WeaponNum);

  
   

}




