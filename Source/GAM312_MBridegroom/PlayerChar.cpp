// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"


// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam")); 
	PlayerCamComp->SetupAttachment(GetMesh(), "head"); 
	PlayerCamComp->bUsePawnControlRotation = true;

	BuildingArray.SetNum(3);
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerUI->UpdateBars(Health, Hunger, Stamina);

	// If player is building, update building part position
	if (isBuilding)
	{
		if (spawnedPart)
		{
			FHitResult HitResult;

			// Get camera position
			FVector StartLocation = PlayerCamComp->GetComponentLocation();

			// Set trace distance
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;

			// Setup collision settings
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.AddIgnoredActor(spawnedPart);

			// Check for placement surface
			if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
			{
				// Place building at hit location
				spawnedPart->SetActorLocation(HitResult.Location);

			}
			else
			{
				// Place building in front of player
				spawnedPart->SetActorLocation(EndLocation);
			}

		}
	}
	
}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// movement input
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);

	// camera input
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	// jump input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerChar::StopJump);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	// Setup Rotate Building input
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);
}

void APlayerChar::MoveForward(float axisValue)
{
	// get forward direction from camera rotation
	FRotator Rotation = GetActorRotation();

	FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::MoveRight(float axisValue)
{
	// get right direction from camera rotation
	FRotator Rotation = GetActorRotation();

	FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::StartJump()
{
	// start jumping
	bPressedJump = true;
}

void APlayerChar::StopJump()
{
	// stop jumping
	bPressedJump = false;
}

void APlayerChar::FindObject()
{
	FHitResult HitResult;

	// Get camera position
	FVector StartLocation = PlayerCamComp->GetComponentLocation();

	// Set trace distance
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	// Setup collision settings
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	if (!isBuilding && !isCrafting)
	{
		PlayAnimMontage(HarvestMontage);
		// Check for hit object
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			// Cast hit actor to resource
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			// Check stamina amount
			if (Stamina > 5.0f)
			{
				if (HitResource)
				{
					
					FString hitName = HitResource->resourceName;
					int resourceValue = FMath::RandRange(
						HitResource->MinResourceAmount,
						HitResource->MaxResourceAmount
					);

					// Remove collected amount
					HitResource->totalResource = HitResource->totalResource - resourceValue;

					// Resource still available
					if (HitResource->totalResource > resourceValue)
					{
						GiveResources(resourceValue, hitName);

						

						
						// Spawn hit effect
						UGameplayStatics::SpawnDecalAtLocation(
							GetWorld(),
							hitDecal,
							FVector(10.0f, 10.0f, 10.0f),
							HitResult.Location,
							FRotator(-90, 0, 0),
							2.0f);

						// Reduce stamina
						SetStamina(-5.0f);
						
					}
					else
					{
						// Remove empty resource
						
						HitResource->Destroy();

						
					}
				}
			}
			
		}
	}
	else
	{
		isBuilding = false;
		if (spawnedPart)
		{
			spawnedPart->SetActorEnableCollision(true); // FINAL PLACED OBJECT
			spawnedPart = nullptr;
		}
	}

}

void APlayerChar::SetHealth(float Amount)
{
	// Keep health below max
	if (Health + Amount <= 100)
	{
		Health = Health + Amount;
	}
	else
		{
		Health = 100.0f;
	}
}

void APlayerChar::SetHunger(float Amount)
{
	// Keep hunger below max
	if (Hunger + Amount <= 100)
	{
		Hunger = Hunger + Amount;
	}
	else
	{
		Hunger = 100.0f;
	}
}

void APlayerChar::SetStamina(float Amount)
{
	// Keep stamina below max
	if (Stamina + Amount <= 100)
	{
		Stamina = Stamina + Amount;
	}
	else
	{
		Stamina = 100.0f;
	}
}

void APlayerChar::DecreaseStats()
{
	// Lower hunger over time
	if (Hunger > 0.0f)
	{
		SetHunger(-1.0f);
	}

	// Restore stamina
	SetStamina(0.5f);

	// Lose health if starving
	if (Hunger <= 0.0f)
	{
		SetHealth(-3.0f);
	}
}

void APlayerChar::GiveResources(float Amount, FString resourceType)
{

	// Add wood amount
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + Amount;
	}

	// Add stone amount
	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + Amount;
	}

	// Add berry amount
	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + Amount;
	}

	ShowResourcePopup(resourceType, Amount);
}
// Updates resources when building
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{    // Subtract used resources
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}

		}
	}
}

// Spawns building part
void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{    // Spawn building part
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);
			
			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildingPartClass, EndLocation, myRot, SpawnParams);

			spawnedPart->SetActorEnableCollision(false);
			spawnedPart->SetActorTickEnabled(true);

			isSuccess = true;

		
		}
		else
		{
			isSuccess = false;
		}
	}
}
// Rotates building part
void APlayerChar::RotateBuilding()
{
	if (isBuilding)
	{
		spawnedPart->AddActorLocalRotation(FRotator(0, 90, 0));
	}
}

