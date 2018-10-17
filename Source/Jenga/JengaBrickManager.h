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
	TowerSnapshot* GetInitialSnapshot();

	bool HasTowerFallen(TowerSnapshot* snapshot);

	void SelectBrick(AJengaBrick* brick);
	void HoverBrick(AJengaBrick* brick);
	void ResetSelections();
	
private:
	AJengaBrick* m_jengaBricks[BRICK_COUNT];		

	UPROPERTY(EditAnywhere)
	float m_thickness;

	UPROPERTY(EditAnywhere)
	float m_spawnZModifier = 1.1;

	UPROPERTY(EditAnywhere)
	float m_spawnXYModifier = 1.05;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJengaBrick> m_brickTemplate;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* m_normalMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* m_selectedMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* m_hoverMaterial;

	FVector* GetLocationSnapshot();
	FRotator* GetRotationSnapshot();

	FVector m_locations[BRICK_COUNT];
	FRotator m_rotations[BRICK_COUNT];

	AJengaBrick* m_selectedBrick = nullptr;
	AJengaBrick* m_hoveredBrick = nullptr;

	bool m_areBricksSpawned = false;

	TowerSnapshot* m_initialSnapshot;

	void SetMaterial(AJengaBrick* brick, bool selected);

	int GetLevel(float height);
	float GetMaxHeight();
	int GetMaxLevel();
	float GetMaxHeight(TowerSnapshot* snapshot);
	int GetMaxLevel(TowerSnapshot* snapshot);
};
