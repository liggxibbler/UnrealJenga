// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaBrickManager.h"
#include "Engine/World.h"

// Sets default values
AJengaBrickManager::AJengaBrickManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJengaBrickManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnBricks();
}

// Called every frame
void AJengaBrickManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJengaBrickManager::SpawnBricks()
{
	UWorld* world = GetWorld();
	for (int i = 0; i < 3 * 18; ++i)
	{
		m_jengaBricks[i] = world->SpawnActor<AJengaBrick>(GetInitialiPosition(i), GetInitialRotation(i));
		m_jengaBricks[i]->SetActorScale3D(FVector(m_thickness * 5, m_thickness * 15, m_thickness * 3) / 100);
	}	
}

void AJengaBrickManager::InitializeBricks()
{
	for (int i = 0; i < 3 * 18; ++i)
	{
		// Set brick poisition and orientation
		m_jengaBricks[i]->SetActorLocation(GetInitialiPosition(i));
		m_jengaBricks[i]->SetActorRotation(GetInitialRotation(i));
		// Activate physics simulation
	}
}

void AJengaBrickManager::DestroyBricks()
{
	for (int i = 0; i < 3 * 18; ++i)
	{
		m_jengaBricks[i]->Destroy();
	}
}

AJengaBrick** AJengaBrickManager::GetBricks()
{
	return m_jengaBricks;
}

FVector* AJengaBrickManager::GetSnapshot()
{
	return nullptr;
}

FVector AJengaBrickManager::GetInitialiPosition(int i)
{
	int level = i / 3;
	int side = i % 3;
	if (level % 2 == 0)
	{
		return FVector((side - 1) * m_thickness * 5, 0, (level + .5) * m_thickness * 3);
	}
	else
	{
		return FVector(0, (side - 1) * m_thickness * 5, (level + .5) * m_thickness * 3);
	}
}

FRotator AJengaBrickManager::GetInitialRotation(int i)
{
	int level = i / 3;
	return FRotator(0, (level % 2 == 0) ? 0 : 90, 0);
}