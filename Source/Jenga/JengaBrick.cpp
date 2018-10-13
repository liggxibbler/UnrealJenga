// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaBrick.h"


// Sets default values
AJengaBrick::AJengaBrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_physics = CreateDefaultSubobject<UPhysicsHandleComponent>("Physics");
	m_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Visual");
}

// Called when the game starts or when spawned
void AJengaBrick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJengaBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

