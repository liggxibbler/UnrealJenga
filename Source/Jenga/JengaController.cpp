// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaController.h"
#include "Engine/World.h"
#include "EngineGlobals.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AJengaController::AJengaController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_turn = 0;
	m_phase = PhaseInit;
}

// Called when the game starts or when spawned
void AJengaController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJengaController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Restting"));
		m_brickManager->InitializeBricks();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Exploding"));
		m_brickManager->Explode();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pop Undo Stack"));
		Undo();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Push Undo Stack"));
		PushUndoStack();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pop Redo Stack"));
		Redo();
	}
	/*if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::P))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Push Redo Stack"));
		m_brickManager->InitializeBricks();
	}*/

}

void AJengaController::AlignBricks()
{
	// Don't forget to set angular and linear velocities to zero
}

void AJengaController::NewGame(int playerCount)
{
	m_playerCount = playerCount;
	m_brickManager->InitializeBricks();
	OnBeginTurn();
}


void AJengaController::OnBeginTurn()
{
	m_phase = PhaseRemoval;
	PushUndoStack();
}
void AJengaController::OnFinishTurn()
{	
	m_turn = (m_turn + 1) % m_playerCount;
	OnBeginTurn();
}

void AJengaController::OnBrickRemoved()
{
	m_phase = PhasePlacement;
}
void AJengaController::OnBrickPlaced()
{
	m_phase = PhaseWait;
}

void AJengaController::PushUndoStack()
{
	m_undoStack.push(m_brickManager->GetSnapshot());
	
	std::string str = "";


	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Restting"));
}

void AJengaController::PushRedoStack(TowerSnapshot* snapshot)
{
	m_redoStack.push(snapshot);
}

void AJengaController::Undo()
{
	if (m_undoStack.empty())
	{
		// Nag about it
	}
	else
	{
		auto snapshot = m_undoStack.top();
		PushRedoStack(snapshot);
		m_brickManager->ApplySnapshot(snapshot);
		m_undoStack.pop();
	}
}

void AJengaController::Redo()
{
	if (m_redoStack.empty())
	{
		// Nag about it
	}
	else
	{
		auto snapshot = m_redoStack.top();
		m_brickManager->ApplySnapshot(snapshot);
		m_redoStack.pop();
	}
}