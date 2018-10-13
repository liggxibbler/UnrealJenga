// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "JengaBrick.generated.h"

UCLASS()
class JENGA_API AJengaBrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJengaBrick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_mesh;

	UPROPERTY(EditAnywhere)
	UPhysicsHandleComponent* m_physics;
};
