// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JengaBrick.h"
#include "JengaBrickManager.generated.h"

UCLASS()
class JENGA_API AJengaBrickManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJengaBrickManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnBricks();
	void InitializeBricks();
	void DestroyBricks();
	AJengaBrick** GetBricks();
	FVector* GetSnapshot();
	FVector GetInitialiPosition(int i);
	FRotator GetInitialRotation(int i);

private:
	AJengaBrick* m_jengaBricks[54];
	int GetLevel(float height);	

	UPROPERTY(EditAnywhere)
	float m_thickness;	
};
