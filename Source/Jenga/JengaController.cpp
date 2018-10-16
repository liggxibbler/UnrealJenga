// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaController.h"


// Sets default values
AJengaController::AJengaController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

void AJengaController::Undo()
{

}

void AJengaController::Redo()
{

}