#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

APlayerCharacter::APlayerCharacter()
{
    // 컨트롤러 회전 활성화
    bUseControllerRotationYaw = true;  
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    // 이동 설정
    GetCharacterMovement()->bOrientRotationToMovement = false;  // 방향 자동 변경 방지
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;

    // 카메라 및 스프링암 설정
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true; // 카메라가 컨트롤러 회전 반영

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}


void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInputComp)
    {
        EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
        EnhancedInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
    }
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MoveInput = Value.Get<FVector2D>();

    if (Controller)
    {
        FRotator ControlRotation = Controller->GetControlRotation();
        FRotator YawRotation(0, ControlRotation.Yaw, 0);

        FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MoveInput.Y);
        AddMovementInput(RightDirection, MoveInput.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}