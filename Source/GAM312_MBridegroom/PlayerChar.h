// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_MBRIDEGROOM_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// moves character forward and backward
	UFUNCTION()
		void MoveForward(float axisValue);

	// moves character left and right
	UFUNCTION()
		void MoveRight(float axisValue);

	// Starts the jump action
	UFUNCTION()
		void StartJump();

	// Stops the jump action
	UFUNCTION()
		void StopJump();


	UFUNCTION()
		void FindObject();

	// player camera component
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamComp;
};
