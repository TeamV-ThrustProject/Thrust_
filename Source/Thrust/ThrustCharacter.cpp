// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrustCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"



// Sets default values
AThrustCharacter::AThrustCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->SetCapsuleSize(55, 135);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(GetCapsuleComponent());
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));

    CameraComponent->bUsePawnControlRotation = true;

    MainWeapon = ARifle::StaticClass();    
    CastMainWeapon();
}

// Called when the game starts or when spawned
void AThrustCharacter::BeginPlay()
{
	Super::BeginPlay();
    
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

        PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AThrustCharacter::StartDash);
        PlayerInputComponent->BindAction("Dash", IE_Released, this, &AThrustCharacter::StopDash);

        PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AThrustCharacter::StartAttack);
        PlayerInputComponent->BindAction("Attack", IE_Released, this, &AThrustCharacter::StopAttack);

        PlayerInputComponent->BindAction("Skill", IE_Pressed, this, &AThrustCharacter::UseSkill);
}

void AThrustCharacter::MoveForward(float Value)
{
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
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

void AThrustCharacter::StartDash()
{
    bDash = true;
}

void AThrustCharacter::StopDash()
{
    bDash = false;
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
}


