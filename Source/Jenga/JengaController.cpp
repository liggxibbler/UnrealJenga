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
	
	m_pc = GetWorld()->GetFirstPlayerController();
	m_pc->bShowMouseCursor = true;
}

// Called every frame
void AJengaController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (m_phase)
	{
	case Phase::PhaseInit:		
		break;
	case Phase::PhaseRemovalSelect:
		if (SelectBrick())
		{
			m_phase = Phase::PhaseRemovalSlide;
		}
		break;
	case Phase::PhaseRemovalSlide:
		if (RemoveBrick())
		{
			OnBrickRemoved();
		}
		break;	
	case Phase::PhasePlacement:
		break;
	case Phase::PhaseWait:
		// Count down from 10 seconds
		m_turnEndTimer -= DeltaTime;
		if (m_turnEndTimer <= 0)
		{
			OnFinishTurn();
		}
		break;
	case Phase::PhaseGameOver:
		break;
	default:
		break;
	}

	if (m_pc->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		switch (m_phase)
		{		
		case Phase::PhaseRemovalSlide:
			OnBrickRemoved();
			break;
		case Phase::PhasePlacement:
			OnBrickPlaced();
			break;
		case Phase::PhaseWait:
			OnFinishTurn();
			break;
		}
	}

	if (m_pc->WasInputKeyJustPressed(EKeys::Enter))
	{
		m_brickManager->Explode();
	}

	if (m_pc->WasInputKeyJustPressed(EKeys::V))
	{
		PushUndoStack();
	}

	if (m_pc->WasInputKeyJustPressed(EKeys::L))
	{
		if (CheckLoseStatus())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Tower has fallen!"));
			GameOver();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Tower still standing"));
		}
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
	m_brickManager->SpawnBricks();
	OnBeginTurn();
}

void AJengaController::GameOver()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Game Over"));
	m_phase = Phase::PhaseGameOver;
	OnGameOver();
}

bool AJengaController::SelectBrick()
{
	// TODO this checks for mouse clicks!
	//pc->WasInputKeyJustReleased(EKeys::LeftMouseButton)

	FHitResult hit;
	auto controller = m_pc->GetHitResultUnderCursor(ECC_WorldDynamic, false, hit);	
	if (hit.bBlockingHit)
	{
		if (nullptr != hit.Actor)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, hit.Actor->GetName());
			
			if (nullptr != dynamic_cast<AJengaBrick*>(hit.Actor.Get()))
			{
				m_brickManager->HoverBrick((AJengaBrick*)hit.GetActor());

				if (m_pc->WasInputKeyJustReleased(EKeys::LeftMouseButton))
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

bool AJengaController::RemoveBrick()
{
	if (m_pc->WasInputKeyJustPressed(EKeys::BackSpace))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Cancelling selection!"));

		m_brickManager->ResetSelections();
		m_phase = Phase::PhaseRemovalSelect;
	}
	else
	{
		FVector direction;
		if (m_pc->IsInputKeyDown(EKeys::Up))
		{
			direction.Y += 1;
		}
		if (m_pc->IsInputKeyDown(EKeys::Down))
		{
			direction.Y -= 1;
		}
		if (m_pc->IsInputKeyDown(EKeys::Right))
		{
			direction.X += 1;
		}
		if (m_pc->IsInputKeyDown(EKeys::Left))
		{
			direction.X -= 1;
		}
		
		direction.Normalize();
		m_brickManager->MoveSelectedBrick(direction);
	}

	return m_brickManager->IsBrickRemoved();
}

void AJengaController::OnBeginTurn()
{
	auto str = FString::FromInt(m_currentPlayer);
	auto str2 = FString("Player ") + str;
	auto i = FCString::Atoi(str.GetCharArray().GetData());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, str2);

	OnBeginTurnEvent();

	m_brickManager->ResetSelections();

	m_phase = PhaseRemovalSelect;	
}
void AJengaController::OnFinishTurn()
{	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Turn finished!"));

	if (CheckLoseStatus())
	{
		GameOver();
	}
	else
	{
		PushUndoStack();
		++m_turn;
		m_currentPlayer = m_turn % m_playerCount;
		OnBeginTurn();
	}
}

void AJengaController::OnBrickRemoved()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Brick removed!"));
	m_phase = PhasePlacement;
}
void AJengaController::OnBrickPlaced()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Brick placed!"));
	m_phase = PhaseWait;
	m_turnEndTimer = m_turnWaitDuration;	
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

bool AJengaController::CheckLoseStatus()
{
	return m_brickManager->HasTowerFallen(LastStableSnapshot());
}

// PRIVATE

TowerSnapshot* AJengaController::LastStableSnapshot()
{
	if (!m_undoStack.empty())
	{
		return m_undoStack.top();
	}
	else
	{
		return m_brickManager->GetInitialSnapshot();
	}
	
}