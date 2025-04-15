// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArmComponent->SetupAttachment(ABasePawn::TurretMesh);
	Camera->SetupAttachment(SpringArmComponent);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());

	if (TankPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(TankMappingContext, 0);
		}
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult);

		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}


void ATank::Move(const FInputActionValue& value)
{
	const float currentvalue = value.Get<float>();
	if (GetController() && currentvalue != 0)
	{
		FVector	forward(0.f);
		forward.X = currentvalue * UGameplayStatics::GetWorldDeltaSeconds(this) * MovementSpeed;
		AddActorLocalOffset(forward, true);
	}

}

void ATank::Turn(const FInputActionValue& value)
{
	float currentvalue = value.Get<float>();
	if (GetController() && currentvalue != 0)
	{
		currentvalue = currentvalue * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
		AddControllerYawInput(currentvalue);
	}
}

void ATank::Fire(const FInputActionValue& value)
{
	bool currentvalue = value.Get<bool>();
	if (currentvalue)
	{
		ABasePawn::Fire();
	}
}
