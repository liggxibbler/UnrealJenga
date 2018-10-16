// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stack>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
		PhaseRemoval,
		PhasePlacement,
		PhaseWait,
	};

private:
	std::stack<TowerSnapshot*> m_undoStack;
	std::stack<TowerSnapshot*> m_redoStack;

	UPROPERTY(EditAnywhere)
	AJengaBrickManager* m_brickManager;

	UPROPERTY(EditAnywhere)
	int m_playerCount;

	Phase m_phase = PhaseRemoval;
	int m_turn = 0;

public:	
	// Sets default values for this actor's properties
	AJengaController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AlignBricks(); // Don't forget to set angular and linear velocities to zero
	void NewGame(int playerCount);

	void OnBeginTurn();
	void OnFinishTurn();
	
	void OnBrickRemoved();
	void OnBrickPlaced();
	

	void Undo();
	void Redo();


};