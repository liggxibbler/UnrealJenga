// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerSnapshot.h"

TowerSnapshot::TowerSnapshot()
{
}

TowerSnapshot::TowerSnapshot(FVector locations[], FRotator rotations[])
{
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		m_snapshot[i].location = locations[i];
		m_snapshot[i].rotation = rotations[i];
	}
}

TowerSnapshot::~TowerSnapshot()
{
}


TowerSnapshot::BrickState* TowerSnapshot::GetSnapshot()
{
	return m_snapshot;
}