// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaController.h"


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

}

void AJengaController::AlignBricks()
{
	// Don't forget to set angular and linear velocities to zero
}

void AJengaController::NewGame(int playerCount)
{
	m_playerCount = playerCount;
	OnBeginTurn();
}


void AJengaController::OnBeginTurn()
{
	m_phase = PhaseRemoval;
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


void AJengaController::Undo()
{
	if (m_undoStack.empty())
	{
		// Nag about it
	}
	else
	{
		auto snapshot = m_undoStack.top();
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