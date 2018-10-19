// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"

#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"

#define RAD2DEG 180 / PI

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

	ResetRotations();
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
	if (!m_isActive)
		return;
	//AddMovementInput(GetActorForwardVector(), value);
	m_theta -= value * m_sensitivity;
	
	if (m_theta > PI / 2)
	{
		m_theta = PI / 2;
	}
	if (m_theta < 0)
	{
		m_theta = 0;
	}

	UpdatePosAndRot();
}

void AMyPawn::RotateRight(float value)
{
	if (!m_isActive)
		return;

	//AddMovementInput(GetActorRightVector(), value);
	m_phi -= value * m_sensitivity;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FVector2D(m_theta, m_phi).ToString());

	if (m_phi > 2 * PI)
	{
		m_phi -= 2 * PI;
	}
	if (m_phi < 0)
	{
		m_phi += 2 * PI;
	}	

	UpdatePosAndRot();
}

void AMyPawn::UpdatePosAndRot()
{
	SetActorLocation(FVector(FMath::Sin(m_theta) * FMath::Cos(m_phi), FMath::Sin(m_theta) * FMath::Sin(m_phi), FMath::Cos(m_theta)) * m_radius);
	//SetActorRotation(FLookAtMatrix(GetActorLocation(), FVector(0, 0, 0), FVector(0, 0, 1)).Rotator());	
	//m_camera->SetWorldRotation(FRotator(180 + m_theta * RAD2DEG, m_phi * RAD2DEG, 0));	
	//auto matrix = FLookAtMatrix(-GetActorLocation(), FVector(0, 0, 0), FVector(0, 1, 0));
	//m_camera->SetWorldRotation(matrix.Rotator());
	auto loc = GetActorLocation();
	auto q1 = FQuat(FVector(0, 0, 1), PI + m_phi);
	auto q2 = FQuat(FVector(-loc.Y, loc.X, 0), -m_theta * 0);
	auto q3 = q1 * q2;
	m_camera->SetWorldRotation(q3);	
}

void AMyPawn::SetRadius(float radius)
{
	m_radius = radius;
}

void AMyPawn::ResetRotations()
{
	m_theta = PI / 6;
	m_phi = 3 * PI / 2;

	UpdatePosAndRot();
}

void AMyPawn::SetActive(bool active)
{
	m_isActive = active;
}