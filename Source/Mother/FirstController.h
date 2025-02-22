// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "FirstController.generated.h"

UCLASS()
class MOTHER_API AFirstController : public APlayerController
{
	GENERATED_BODY()

public:
	AFirstController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IAMove;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IALook;
};

