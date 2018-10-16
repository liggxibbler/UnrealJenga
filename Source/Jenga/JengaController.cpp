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
		m_brickManager->InitializeBricks();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
	{
		m_brickManager->Explode();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::U))
	{
		Undo();
	}
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::V))
	{
		PushUndoStack();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	{
		Redo();
	}
	
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::N))
	{
		NewGame(m_playerCount);
	}

}

void AJengaController::ClearStacks()
{
	while (!m_undoStack.empty())
	{
		auto top = m_undoStack.top();
		delete top;
		m_undoStack.pop();
	}

	while (!m_redoStack.empty())
	{
		auto top = m_redoStack.top();
		delete top;
		m_redoStack.pop();
	}
}

void AJengaController::AlignBricks()
{
	// Don't forget to set angular and linear velocities to zero
}

void AJengaController::NewGame(int playerCount)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Starting new game"));
	ClearStacks();
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Push Undo Stack"));
	m_undoStack.push(m_brickManager->GetSnapshot());
}

void AJengaController::PushRedoStack()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Push Redo Stack"));
	m_redoStack.push(m_brickManager->GetSnapshot());
}

void AJengaController::Undo()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pop Undo Stack"));
	if (m_undoStack.empty())
	{
		// Nag about it
	}
	else
	{
		PushRedoStack();
		auto snapshot = m_undoStack.top();
		m_brickManager->ApplySnapshot(snapshot);		
		m_undoStack.pop();
	}
}

void AJengaController::Redo()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pop Redo Stack"));
	if (m_redoStack.empty())
	{
		// Nag about it
	}
	else
	{
		PushUndoStack();
		auto snapshot = m_redoStack.top();
		m_brickManager->ApplySnapshot(snapshot);
		m_redoStack.pop();
	}
}