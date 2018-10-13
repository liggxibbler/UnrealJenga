// Fill out your copyright notice in the Description page of Project Settings.

#include "AMoveX.h"


// Sets default values
AAMoveX::AAMoveX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAMoveX::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAMoveX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_doesMove)
	{
		FVector forward = GetActorForwardVector();
		FVector position = GetActorLocation();
		position += forward * 5.0 * DeltaTime;
		SetActorLocation(position);
	}
}

