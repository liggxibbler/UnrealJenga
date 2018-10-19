// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerSnapshot.h"
#include "EngineGlobals.h"

TowerSnapshot::TowerSnapshot()
{
}

TowerSnapshot::TowerSnapshot(FVector locations[], FRotator rotations[], int currentPlayer, int currentTurn, int currentPhase)
	: m_player(currentPlayer), m_turn(currentTurn), m_phase(currentPhase)
{
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, locations[i].ToString() + FString("	,	") + rotations[i].ToString());
		m_snapshot[i].location = locations[i];
		m_snapshot[i].rotation = rotations[i];
	}
}

TowerSnapshot::~TowerSnapshot()
{
}


TowerSnapshot::BrickState TowerSnapshot::GetSnapshot(int idx)
{
	return m_snapshot[idx];
}

int TowerSnapshot::GetPlayer()
{
	return m_player;
}

int TowerSnapshot::GetTurn()
{
	return m_turn;
}

int TowerSnapshot::GetPhase()
{
	return m_phase;
}