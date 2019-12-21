
#include "Simulation/Drone.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/World.h"
#include "CommandGameState.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Camera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	ACommandGameState* GameState = (ACommandGameState*)GetWorld()->GetGameState();
	GameState->RecordDrone(this);
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalOffset(totalSpeed, true);
}

void ADrone::ReceiveCommand(FCommand Command)
{
	UE_LOG(LogTemp, Warning, TEXT("%s : %f"), *Command.CommandName, Command.Value);
	Command.Value = Command.Value * 10;
	
	if (Command.CommandName == "up")
	{
		positiveSpeed.Z = Command.Value;
	}
	else if (Command.CommandName == "up stop")
	{
		positiveSpeed.Z = 0;
	}
	else if (Command.CommandName == "down")
	{
		negativeSpeed.Z = Command.Value;
	}
	else if (Command.CommandName == "down stop")
	{
		negativeSpeed.Z = 0;
	}
	else if (Command.CommandName == "left")
	{
		negativeSpeed.Y = Command.Value;
	}
	else if (Command.CommandName == "left stop")
	{
		negativeSpeed.Y = 0;
	}
	else if (Command.CommandName == "right")
	{
		positiveSpeed.Y = Command.Value;
	}
	else if (Command.CommandName == "right stop")
	{
		positiveSpeed.Y = 0;
	}
	else if (Command.CommandName == "forward")
	{
		positiveSpeed.X = Command.Value;
	}
	else if (Command.CommandName == "forward stop")
	{
		positiveSpeed.X = 0;
	}
	else if (Command.CommandName == "backward")
	{
		negativeSpeed.X = Command.Value;
	}
	else if (Command.CommandName == "backward stop")
	{
		negativeSpeed.X = 0;
	}

	totalSpeed = positiveSpeed - negativeSpeed;
}

