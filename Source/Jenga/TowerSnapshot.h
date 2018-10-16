// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define LEVEL_COUNT 18
#define BRICK_COUNT 3 * LEVEL_COUNT

#include "CoreMinimal.h"

/**
 * 
 */
class JENGA_API TowerSnapshot
{
public:
	TowerSnapshot();
	TowerSnapshot(FVector locations[], FRotator rotations[]);
	
	~TowerSnapshot();

	struct BrickState
	{
		FVector location;
		FRotator rotation;
	};

	BrickState* GetSnapshot();

private:
	BrickState m_snapshot[BRICK_COUNT];
};
