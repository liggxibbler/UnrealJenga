// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaBrickManager.h"

#include <string>

#include "Engine/World.h"
#include "EngineGlobals.h"
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

}

// Called every frame
void AJengaBrickManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* world = GetWorld();
}

void AJengaBrickManager::SpawnBricks()
{
	if (m_areBricksSpawned)
	{
		InitializeBricks();
		return;
	}

	m_areBricksSpawned = true;

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

	m_initialSnapshot = GetSnapshot();
}

void AJengaBrickManager::InitializeBricks()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Restting"));
	ApplySnapshot(m_initialSnapshot);
}

void AJengaBrickManager::Explode()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Exploding"));
	for (int i = 0; i < BRICK_COUNT - 1; ++i)
	{
		m_jengaBricks[i]->m_mesh->SetPhysicsLinearVelocity(m_jengaBricks[i + 1]->GetActorLocation());
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
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, m_jengaBricks[i]->GetActorLocation().ToString());
		m_locations[i] = m_jengaBricks[i]->GetActorLocation();
	}
	return m_locations;
}

FRotator* AJengaBrickManager::GetRotationSnapshot()
{
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, m_jengaBricks[i]->GetActorRotation().ToString());
		m_rotations[i] = m_jengaBricks[i]->GetActorRotation();
	}
	return m_rotations;
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
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, (snapshot->GetSnapshot(i).location).ToString() + FString("	,	") + (snapshot->GetSnapshot(i).rotation).ToString());
		m_jengaBricks[i]->SetActorLocation(snapshot->GetSnapshot(i).location);
		m_jengaBricks[i]->SetActorRotation(snapshot->GetSnapshot(i).rotation);
		m_jengaBricks[i]->m_mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
		m_jengaBricks[i]->m_mesh->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
	}
}

TowerSnapshot* AJengaBrickManager::GetInitialSnapshot()
{
	return m_initialSnapshot;
}

bool AJengaBrickManager::HasTowerFallen(TowerSnapshot* snapshot)
{
	float maxHeight = GetMaxHeight(snapshot);
	float maxLevel = GetMaxLevel(snapshot);
	float diffSqr = m_thickness * 3 * m_thickness * 3;

	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		if (GetLevel(m_jengaBricks[i]->GetActorLocation().Z) != maxLevel)
		{
			float distSqr = FVector::DistSquared(snapshot->GetSnapshot(i).location, m_jengaBricks[i]->GetActorLocation());
			if (distSqr > diffSqr)
			{
				return true;
			}
		}
	}

	return false;
}


void AJengaBrickManager::SelectBrick(AJengaBrick* brick)
{
	m_selectedBrick = brick;
	m_selectedInitialLocation = brick->GetActorLocation();
	m_selectedBrick->SetMaterial(m_selectedMaterial);
}
void AJengaBrickManager::HoverBrick(AJengaBrick* brick)
{
	if (brick != m_hoveredBrick)
	{
		if (nullptr != m_hoveredBrick)
		{
			m_hoveredBrick->SetMaterial(m_normalMaterial);			
		}

		m_hoveredBrick = brick;

		if (nullptr != m_hoveredBrick)
		{
			m_hoveredBrick->SetMaterial(m_hoverMaterial);
		}
	}	
}

void AJengaBrickManager::ResetSelections()
{
	if (nullptr != m_selectedBrick)
	{
		SetMaterial(m_selectedBrick, false);
		m_selectedBrick = nullptr;
	}
}
void AJengaBrickManager::MoveSelectedBrick(FVector direction)
{
	FVector location = m_selectedBrick->GetActorLocation();
	FVector displacement = direction.X * m_selectedBrick->GetActorForwardVector() +
		direction.Y * m_selectedBrick->GetActorRightVector();
	m_selectedBrick->SetActorLocation(location + m_brickSpeed * displacement);
}
bool AJengaBrickManager::IsBrickRemoved()
{
	if (nullptr != m_selectedBrick)
	{
		FVector diff = m_selectedBrick->GetActorLocation() - m_selectedInitialLocation;		
		return diff.SizeSquared() >= 300 * m_thickness * m_thickness; // 300 = 250 + 50. 250 is smallest squared distance where brick is definitely not overlapping with its initial bounding box
		// TODO a more precise (and decent) check would be in the brick's local coordinate system - will do that if there's time
	}

	return false;
}

void AJengaBrickManager::SetMaterial(AJengaBrick* brick, bool selected)
{
	brick->m_mesh->SetMaterial(0, selected ? m_selectedMaterial : m_normalMaterial);
}

int AJengaBrickManager::GetLevel(float height)
{
	return (height - m_thickness * 3 * .5) / (m_thickness * 3);
}

float AJengaBrickManager::GetMaxHeight()
{
	float max = 0;
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		if (m_jengaBricks[i]->GetActorLocation().Z > max)
		{
			max = m_jengaBricks[i]->GetActorLocation().Z;
		}
	}

	return max;
}

int AJengaBrickManager::GetMaxLevel()
{
	return GetLevel(GetMaxHeight());
}

float AJengaBrickManager::GetMaxHeight(TowerSnapshot* snapshot)
{
	float max = 0;
	for (int i = 0; i < BRICK_COUNT; ++i)
	{
		if (snapshot->GetSnapshot(i).location.Z > max)
		{
			max = snapshot->GetSnapshot(i).location.Z;
		}
	}

	return max;
}

int AJengaBrickManager::GetMaxLevel(TowerSnapshot* snapshot)
{
	return GetLevel(GetMaxHeight(snapshot));
}