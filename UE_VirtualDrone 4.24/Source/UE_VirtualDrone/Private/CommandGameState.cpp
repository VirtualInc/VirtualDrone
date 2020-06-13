// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandGameState.h"
#include "Drone.h"

ACommandGameState::ACommandGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACommandGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!isAlive) return;

	int Length = CommandsQueue.Num();
	if (Length > 0)
	{
		Length = (Length < 5 ? Length : 5);
		for (int i = 0; i < Length; i++)
			SendCommand();
	}
}

void ACommandGameState::AddCommand(FCommand Command)
{
	if (!isAlive) return;

	CommandsQueue.Push(Command);
}

void ACommandGameState::SendCommand()
{
	FCommand Command = CommandsQueue.Pop();

	if (Drone)
		Drone->ReceiveCommand(Command);
}

void ACommandGameState::RecordDrone(ADrone* InDrone)
{
	Drone = InDrone;
}

void ACommandGameState::SetAliveState(bool alive)
{
	isAlive = alive;

	if (!alive) {
		CommandsQueue.Empty();
		CommandsQueue.Push(FCommand(EMovements::StopAll, 0));
		SendCommand();
	}
}
