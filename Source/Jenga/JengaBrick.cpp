// Fill out your copyright notice in the Description page of Project Settings.

#include "JengaBrick.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AJengaBrick::AJengaBrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//m_physics = CreateDefaultSubobject<UBoxComponent>("Physics");
	//m_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Visual");
	//m_mesh->SetSimulatePhysics(false);
}

void AJengaBrick::SetMaterial(UMaterialInterface* material)
{
	m_mesh->SetMaterial(0, material);
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

