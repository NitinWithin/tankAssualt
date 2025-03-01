// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputActionValue.h"

#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:

	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* TankMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* RotateTurretAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireAction;

	
	void Move(const FInputActionValue& value);
	void Turn(const FInputActionValue& value);
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 500;
	
	UPROPERTY(EditAnywhere)
	float TurnRate = 100;

	APlayerController* TankPlayerController;

	void Fire(const FInputActionValue& value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APlayerController* GetPlayerController() const { return TankPlayerController; }

};
