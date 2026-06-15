// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "ObjectiveWidget.h"
#include "PlayerWidget.h"
#include "Animation/AnimMontage.h"
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

	UFUNCTION()
		void ToggleGridSnap();

	// player camera component
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* PlayerCamComp;


	// Player health value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	// Player hunger value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	// Player stamina value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;

		// Player resource values
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	// Stores all resource values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;

	// Stores resource names
	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	// Decal used for hit effect
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> BuildingArray;

	UPROPERTY()
		bool isBuilding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildingPartClass;

	UPROPERTY()
		ABuildingPart* spawnedPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* PlayerUI;

	//Objective widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObjectiveWidget* objWidget;

	UPROPERTY()
		float objectsBuilt = 0.0f;

	UPROPERTY()
		float matsCollected = 0.0f;

	// Changes player health
	UFUNCTION(BlueprintCallable)
		void SetHealth(float Amount);

	// Changes player hunger
	UFUNCTION(BlueprintCallable)
		void SetHunger(float Amount);

	// Changes player stamina
	UFUNCTION(BlueprintCallable)
		void SetStamina(float Amount);

	// Updates player stats over time
	UFUNCTION()
		void DecreaseStats();

	// Adds resources to inventory
	UFUNCTION()
		void GiveResources(float Amount, FString resourceType);

		// Updates resources when building
	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);
		// Spawns building part
	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);
		// Rotates building part
	UFUNCTION()
		void RotateBuilding();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* HarvestMontage;

	UPROPERTY(BlueprintReadWrite, Category = "State")
		bool isCrafting = false;

	UFUNCTION(BlueprintImplementableEvent)
		void ShowResourcePopup(const FString& ResourceType, float Amount);

	UPROPERTY()
		float GridSize = 100.f;

	UPROPERTY()
		bool bCanPlace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
		bool bUseGridSnap = true;

	UFUNCTION()
		void CancelBuilding();

	UPROPERTY()
		int BuildingTypeBeingPlaced = 0;


};
