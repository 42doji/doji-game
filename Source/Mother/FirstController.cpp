// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstController.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

AFirstController::AFirstController()
{
    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;
    
}

void AFirstController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

void AFirstController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
    if (EnhancedInputComp)
    {
        EnhancedInputComp->BindAction(IAMove, ETriggerEvent::Triggered, this, &AFirstController::Move);
        EnhancedInputComp->BindAction(IALook, ETriggerEvent::Triggered, this, &AFirstController::Look);
    }
}

void AFirstController::Move(const FInputActionValue& Value)
{
    FVector2D MoveInput = Value.Get<FVector2D>();

    if (APawn* ControlledPawn = GetPawn())
    {
        FRotator ControlRot = GetControlRotation();
        FRotator YawRotation(0, ControlRot.Yaw, 0);

        FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        ControlledPawn->AddMovementInput(ForwardDirection, MoveInput.X);
        ControlledPawn->AddMovementInput(RightDirection, MoveInput.Y);
    }
}

void AFirstController::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();

    AddYawInput(LookInput.X);  // 마우스 X축 → 좌우 회전
    AddPitchInput(LookInput.Y); // 마우스 Y축 → 위아래 회전
}


