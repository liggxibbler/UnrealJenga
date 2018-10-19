// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MyPawn.generated.h"

UCLASS()
class JENGA_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	UPROPERTY(EditAnywhere)
		UCameraComponent* m_camera;

	UPROPERTY(EditAnywhere, Category = "Params")
		float m_radius = 600.0;
	UPROPERTY(EditAnywhere, Category = "Params")
		float m_sensitivity = 1.0;

	float m_theta = 30;
	float m_phi = 270;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RotateUp(float value);
	void RotateRight(float value);
	
	void UpdatePosAndRot();
};
