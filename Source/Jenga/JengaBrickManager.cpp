// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaBrickManager.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

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

	UWorld* world = GetWorld();

	if (m_spawnCount < 18)
	{
		//m_jengaBricks[m_spawnCount] = world->SpawnActor<AJengaBrick>(m_brickTemplate, GetInitialiPosition(m_spawnCount), GetInitialRotation(m_spawnCount));
		//m_jengaBricks[m_spawnCount]->SetActorScale3D(FVector(m_thickness * 5, m_thickness * 15, m_thickness * 3) / 100);
		for (int i = 0; i < 3; ++i)
		{
			//m_jengaBricks[(17 - m_spawnCount) * 3 + i]->m_mesh->SetSimulatePhysics(true);
		}
		m_spawnCount++;
	}
}

void AJengaBrickManager::SpawnBricks()
{
	UWorld* world = GetWorld();
	for (int i = 0; i < 3 * 18; ++i)
	{
		m_jengaBricks[i] = world->SpawnActor<AJengaBrick>(m_brickTemplate, GetInitialiPosition(i), GetInitialRotation(i));
		m_jengaBricks[i]->SetActorScale3D(FVector(m_thickness * 5, m_thickness * 15, m_thickness * 3) / 100);
		TArray<UStaticMeshComponent*> meshArray;
		m_jengaBricks[i]->GetComponents<UStaticMeshComponent>(meshArray);
		m_jengaBricks[i]->m_mesh = meshArray[0];
		//m_jengaBricks[i]->m_mesh->SetSimulatePhysics(false);		
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
		return FVector((side - 1) * m_thickness * m_spawnXYModifier * 5, 0, (level + .5) * m_thickness * 3 + (level + 1) * m_spawnZModifier);
	}
	else
	{
		return FVector(0, (side - 1) * m_thickness * m_spawnXYModifier * 5, (level + .5) * m_thickness * 3 + (level + 1) * m_spawnZModifier);
	}
}

FRotator AJengaBrickManager::GetInitialRotation(int i)
{
	int level = i / 3;
	return FRotator(0, (level % 2 == 0) ? 0 : 90, 0);
}