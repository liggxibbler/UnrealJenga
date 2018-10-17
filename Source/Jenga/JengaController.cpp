// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaController.h"
#include "Engine/World.h"
#include "EngineGlobals.h"


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
	
	pc = GetWorld()->GetFirstPlayerController();
	pc->bShowMouseCursor = true;
}

// Called every frame
void AJengaController::Tick(float DeltaTime)
{
	switch (m_phase)
	{
	case Phase::PhaseInit:
		//	TODO This will later become a UI event
		if (pc->WasInputKeyJustPressed(EKeys::S))
		{
			NewGame(m_playerCount);
		}
		break;
	case Phase::PhaseRemovalSelect:
		if (SelectBrick())
		{
			m_phase = Phase::PhaseRemovalSlide;
		}
		break;
	case Phase::PhaseRemovalSlide:
		if (pc->WasInputKeyJustPressed(EKeys::BackSpace))
		{
			m_brickManager->ResetSelections();
			m_phase = Phase::PhaseRemovalSelect;
		}
		break;	
	case Phase::PhasePlacement:
		break;
	case Phase::PhaseWait:
		break;
	default:
		break;
	}

	Super::Tick(DeltaTime);

	if (pc->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		m_brickManager->InitializeBricks();
	}

	if (pc->WasInputKeyJustPressed(EKeys::Enter))
	{
		m_brickManager->Explode();
	}

	if (pc->WasInputKeyJustPressed(EKeys::U))
	{
		Undo();
	}
	if (pc->WasInputKeyJustPressed(EKeys::V))
	{
		PushUndoStack();
	}

	if (pc->WasInputKeyJustPressed(EKeys::R))
	{
		Redo();
	}
	
	if (pc->WasInputKeyJustPressed(EKeys::N))
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

void AJengaController::GameOver()
{

}

bool AJengaController::SelectBrick()
{
	// TODO this checks for mouse clicks!
	//pc->WasInputKeyJustReleased(EKeys::LeftMouseButton)

	FHitResult hit;
	auto controller = GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_WorldDynamic, false, hit);
	if (hit.bBlockingHit)
	{
		if (nullptr != hit.Actor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, hit.Actor->GetName());
			
			if (nullptr != dynamic_cast<AJengaBrick*>(hit.Actor.Get()))
			{
				m_brickManager->HoverBrick((AJengaBrick*)hit.GetActor());

				if (pc->WasInputKeyJustReleased(EKeys::LeftMouseButton))
				{				
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Brick selected!"));
					m_brickManager->SelectBrick((AJengaBrick*)hit.GetActor());
					return true;
				}
			}
		}
		else
		{ 
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("NO HIT"));
		}
	}
	return false;
}


void AJengaController::OnBeginTurn()
{
	m_brickManager->ResetSelections();

	m_phase = PhaseRemovalSelect;
	PushUndoStack();
}
void AJengaController::OnFinishTurn()
{	
	++m_turn;
	m_currentPlayer = m_turn % m_playerCount;
	OnBeginTurn();
}

void AJengaController::OnBrickRemoved()
{
	m_phase = PhasePlacement;
}
void AJengaController::OnBrickPlaced()
{
	m_phase = PhaseWait;
	// TODO
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
		--m_turn;
		m_currentPlayer = m_turn % m_playerCount;

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
		++m_turn;
		m_currentPlayer = m_turn % m_playerCount;

		PushUndoStack();
		auto snapshot = m_redoStack.top();
		m_brickManager->ApplySnapshot(snapshot);
		m_redoStack.pop();
	}
}