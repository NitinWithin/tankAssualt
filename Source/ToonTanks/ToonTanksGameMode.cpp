// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
			GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		--TargetTowers;

		if (TargetTowers == 0)
		{
			GameOver(true);
		}
	}
}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowersCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	StartGame();

	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;

		FTimerDelegate StartGameTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true);

		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle,
			StartGameTimerDelegate,
			StartDelay,
			false
		);
	}
}

int32 AToonTanksGameMode::GetTargetTowersCount()
{
	TArray<AActor*> TowerActors;

	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowerActors);

	if (!TowerActors.IsEmpty())
	{
		return TowerActors.Num();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TowerActors is Empty"));
		return 0;
	}
}
