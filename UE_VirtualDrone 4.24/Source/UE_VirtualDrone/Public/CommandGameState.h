// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Movements.h"
#include "CommandGameState.generated.h"


struct FCommand;
class ADrone;

USTRUCT()
struct FCommand
{
	GENERATED_BODY()

		UPROPERTY()
		EMovements Command;

	UPROPERTY()
		float Value;

	FCommand(EMovements _Command, float InValue)
	{
		Command = _Command;
		Value = InValue;
	}

	FCommand()
	{
		Command = EMovements::TForward;
		Value = 0;
	}
};

UCLASS()
class UE_VIRTUALDRONE_API ACommandGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACommandGameState();

	virtual void Tick(float DeltaSeconds) override;

	TArray<FCommand> CommandsQueue;

	void  AddCommand(FCommand Command);
	void  SendCommand();

	void RecordDrone(ADrone* InDrone);

	void SetAliveState(bool alive);

private:
	ADrone* Drone;
	bool isAlive;
};
