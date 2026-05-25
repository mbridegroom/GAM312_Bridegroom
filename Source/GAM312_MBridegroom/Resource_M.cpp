// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource_M.h"

// Sets default values
AResource_M::AResource_M()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	
	PrimaryActorTick.bCanEverTick = true;
	// Create text component
	ResourceNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render"));
	// Create mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Set mesh as root
	RootComponent = Mesh;
	// Attach text to mesh
	ResourceNameText->SetupAttachment(Mesh);


}

// Called when the game starts or when spawned
void AResource_M::BeginPlay()
{
	Super::BeginPlay();

	// Convert name to text
	tempText = tempText.FromString(resourceName);

	// Display resource name
	ResourceNameText->SetText(tempText);


}

// Called every frame
void AResource_M::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

