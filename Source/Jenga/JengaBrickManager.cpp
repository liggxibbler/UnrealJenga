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
}

void AJengaBrickManager::SpawnBricks()
{
	UWorld* world = GetWorld();
	for (int i = 0; i < BRICK_COUNT; ++i)
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
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		// Set brick poisition and orientation
		m_jengaBricks[i]->SetActorLocation(GetInitialiPosition(i));
		m_jengaBricks[i]->SetActorRotation(GetInitialRotation(i));
		m_jengaBricks[i]->m_mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		m_jengaBricks[i]->m_mesh->SetPhysicsAngularVelocity(FVector(0, 0, 0));
		// Activate physics simulation
	}
}

void AJengaBrickManager::DestroyBricks()
{
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		m_jengaBricks[i]->Destroy();
	}
}

AJengaBrick** AJengaBrickManager::GetBricks()
{
	return m_jengaBricks;
}

FVector* AJengaBrickManager::GetLocationSnapshot()
{
	FVector* ret = new FVector[BRICK_COUNT];
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		ret[i] = m_jengaBricks[i]->GetActorLocation();
	}
	return ret;
}

FRotator* AJengaBrickManager::GetRotationSnapshot()
{
	FRotator* ret = new FRotator[BRICK_COUNT];
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		ret[i] = m_jengaBricks[i]->GetActorRotation();
	}
	return ret;
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

TowerSnapshot* AJengaBrickManager::GetSnapshot()
{
	return new TowerSnapshot(GetLocationSnapshot(), GetRotationSnapshot());
}

void AJengaBrickManager::ApplySnapshot(TowerSnapshot* snapshot)
{
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		m_jengaBricks[i]->SetActorLocation(snapshot[i].GetSnapshot()->location);
		m_jengaBricks[i]->SetActorRotation(snapshot[i].GetSnapshot()->rotation);
		m_jengaBricks[i]->m_mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		m_jengaBricks[i]->m_mesh->SetPhysicsAngularVelocity(FVector(0, 0, 0));
	}
}

int AJengaBrickManager::GetLevel(float height)
{
	return (height - m_thickness * 3 * .5) / (m_thickness * 3);
}