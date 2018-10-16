// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JengaBrick.h"
#include "TowerSnapshot.h"
#include "JengaBrickManager.generated.h"

UCLASS()
class JENGA_API AJengaBrickManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJengaBrickManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnBricks();
	void InitializeBricks();
	void Explode();
	void DestroyBricks();
	AJengaBrick** GetBricks();
	
	FVector GetInitialiPosition(int i);
	FRotator GetInitialRotation(int i);

	TowerSnapshot* GetSnapshot();
	void ApplySnapshot(TowerSnapshot* snapshot);

private:
	AJengaBrick* m_jengaBricks[BRICK_COUNT];
	int GetLevel(float height);	

	UPROPERTY(EditAnywhere)
	float m_thickness;

	UPROPERTY(EditAnywhere)
	float m_spawnZModifier = 1.1;

	UPROPERTY(EditAnywhere)
	float m_spawnXYModifier = 1.05;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJengaBrick> m_brickTemplate;

	FVector* GetLocationSnapshot();
	FRotator* GetRotationSnapshot();
};
