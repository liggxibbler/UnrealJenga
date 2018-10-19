// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define LEVEL_COUNT 18
#define BRICK_COUNT 54

#include "CoreMinimal.h"

/**
 * 
 */
class JENGA_API TowerSnapshot
{
public:
	TowerSnapshot();
	TowerSnapshot(FVector locations[], FRotator rotations[], int currentPlayer, int currentTurn, int currentPhase);
	
	~TowerSnapshot();

	struct BrickState
	{
		FVector location;
		FRotator rotation;
	};

	BrickState GetSnapshot(int idx);

	int GetPlayer();
	int GetTurn();
	int GetPhase();

	
private:
	BrickState m_snapshot[BRICK_COUNT];
	int m_player;
	int m_turn;
	int m_phase;
};
