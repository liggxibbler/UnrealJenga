// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stack>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#include "JengaBrickManager.h"

#include "JengaController.generated.h"

UCLASS()
class JENGA_API AJengaController : public AActor
{
	GENERATED_BODY()
	
public:
	enum Phase
	{
		PhaseInit,
		PhaseRemovalSelect,
		PhaseRemovalSlide,
		PhasePlacement,
		PhaseWait,
		PhaseGameOver,
	};

private:
	std::stack<TowerSnapshot*> m_undoStack;
	std::stack<TowerSnapshot*> m_redoStack;
	
	UPROPERTY(EditAnywhere)
		AJengaBrickManager* m_brickManager;

	UPROPERTY(EditAnywhere)
		AActor* m_camera;

	UPROPERTY(EditAnywhere)
		int m_playerCount = 1;

	UPROPERTY(EditAnywhere)
		float m_cameraOffset = 200;

	Phase m_phase = PhaseInit;
	int m_turn = 0;
	int m_currentPlayer = 0;

	APlayerController* m_pc = nullptr;

	const float m_turnWaitDuration = 10.0;
	float m_turnEndTimer;

	TowerSnapshot* LastStableSnapshot();
	void SwitchToPlacementCamera();
	void SwitchToRemovalCamera();

public:	
	// Sets default values for this actor's properties
	AJengaController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ClearStacks();
	void AlignBricks(); // Don't forget to set angular and linear velocities to zero
	
	UFUNCTION(BlueprintCallable, Category = "GameController")
		void NewGame(int playerCount);
	UFUNCTION(BlueprintImplementableEvent, Category = "GameControllerEvents")
		void OnGameOver();
	void GameOver();

	bool SelectBrick();
	bool RemoveBrick();
	bool PlaceBrick();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameControllerEvents")
		void OnBeginTurnEvent();
	void OnBeginTurn();
	void OnFinishTurn();
	
	void OnBrickRemoved();
	void OnBrickPlaced();	

	void PushUndoStack();
	void PushRedoStack();
	UFUNCTION(BlueprintCallable, Category = "GameController")
		void Undo();
	UFUNCTION(BlueprintCallable, Category = "GameController")
		void Redo();

	bool CheckLoseStatus();
};
