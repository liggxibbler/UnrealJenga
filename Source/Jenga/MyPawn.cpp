// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"

#define PI 3.141592

#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"

#include "EngineGlobals.h"

#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawn");

	m_camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	m_camera->SetupAttachment(RootComponent);

	m_theta = PI / 6;
	m_phi = 3 * PI / 2;

	SetActorLocation(FVector(FMath::Sin(m_theta) * FMath::Cos(m_phi), FMath::Sin(m_theta) * FMath::Sin(m_phi), FMath::Cos(m_theta)) * m_radius);
	SetActorRotation(FLookAtMatrix(GetActorLocation(), FVector(0,0,0), FVector(0,0,1)).Rotator());
	//m_camera->SetWorldRotation(FLookAtMatrix(GetActorLocation(), FVector(0, 0, 0), FVector(0, 0, 1)).Rotator());
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::RotateUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::RotateRight);

}

void AMyPawn::RotateUp(float value)
{
	//AddMovementInput(GetActorForwardVector(), value);
	m_theta -= value * m_sensitivity;
	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FVector2D(m_theta, m_phi).ToString());
	
	if (m_theta > PI / 2)
	{
		m_theta = PI / 2;
	}
	if (m_theta < 0)
	{
		m_theta = 0;
	}

	SetActorLocation(FVector(FMath::Sin(m_theta) * FMath::Cos(m_phi), FMath::Sin(m_theta) * FMath::Sin(m_phi), FMath::Cos(m_theta)) * m_radius);
	SetActorRotation(FLookAtMatrix(GetActorLocation(), FVector(0, 0, 0), FVector(0, 0, 1)).Rotator());
	//m_camera->SetWorldRotation(FLookAtMatrix(GetActorLocation(), FVector(0, 0, 0), FVector(0, 0, 1)).Rotator());
}

void AMyPawn::RotateRight(float value)
{
	//AddMovementInput(GetActorRightVector(), value);
	m_phi -= value * m_sensitivity;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FVector2D(m_theta, m_phi).ToString());

	if (m_phi > 2 * PI)
	{
		m_phi -= 2 * PI;
	}
	if (m_phi < 0)
	{
		m_phi += 2 * PI;
	}
	SetActorLocation(FVector(FMath::Sin(m_theta) * FMath::Cos(m_phi), FMath::Sin(m_theta) * FMath::Sin(m_phi), FMath::Cos(m_theta)) * m_radius);
	SetActorRotation(FLookAtMatrix(GetActorLocation(), FVector(0, 0, 0), FVector(0, 0, 1)).Rotator());
	//m_camera->SetWorldRotation(FLookAtMatrix(GetActorLocation(), FVector(0, 0, 0), FVector(0, 0, 1)).Rotator());
}

