// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312_MBRIDEGROOM_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Resource type name
	UPROPERTY(EditAnywhere)
		FString resourceName = "Wood";

	// Amount given per hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int resourceAmount = 5;

	// Total available resource
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int totalResource = 100;

	// Stores display text
	UPROPERTY()
		FText tempText;

	// Displays resource name
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ResourceNameText;

	// Resource mesh component
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	// Resource mesh asset
	UPROPERTY(EditAnywhere)
		UStaticMesh* resourceMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int MinResourceAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
		int MaxResourceAmount = 5;



};