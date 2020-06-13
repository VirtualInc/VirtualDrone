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
	AddActorLocalRotation(totalRotation, true);
}

void ADrone::ReceiveCommand(FCommand Command)
{
	switch (Command.Command)
	{
	case EMovements::StopAll:
		positiveSpeed = FVector::ZeroVector;
		negativeSpeed = FVector::ZeroVector;
		positiveRotation = FRotator::ZeroRotator;
		negativeRotation = FRotator::ZeroRotator;
		break;
	case EMovements::TUp:
		UE_LOG(LogTemp, Error, TEXT("Up"));
		positiveSpeed.Z = Command.Value;
		break;
	case EMovements::TUpStop:
		UE_LOG(LogTemp, Error, TEXT("Up Stop"));
		positiveSpeed.Z = 0;
		break;

	case EMovements::TDown:
		UE_LOG(LogTemp, Error, TEXT("Down"));
		negativeSpeed.Z = Command.Value;
		break;
	case EMovements::TDownStop:
		UE_LOG(LogTemp, Error, TEXT("Down Stop"));
		negativeSpeed.Z = 0;
		break;

	case EMovements::TLeft:
		negativeSpeed.Y = Command.Value;
		break;
	case EMovements::TLeftStop:
		negativeSpeed.Y = 0;
		break;

	case EMovements::TRight:
		positiveSpeed.Y = Command.Value;
		break;
	case EMovements::TRightStop:
		positiveSpeed.Y = 0;
		break;

	case EMovements::TForward:
		positiveSpeed.X = Command.Value;
		break;
	case EMovements::TForwardStop:
		positiveSpeed.X = 0;
		break;

	case EMovements::TBackward:
		negativeSpeed.X = Command.Value;
		break;
	case EMovements::TBackwardStop:
		negativeSpeed.X = 0;
		break;



	case EMovements::RLeft:
		positiveRotation.Yaw = Command.Value;
		break;
	case EMovements::RLeftStop:
		positiveRotation.Yaw = 0;
		break;

	case EMovements::RRight:
		negativeRotation.Yaw = Command.Value;
		break;
	case EMovements::RRightStop:
		negativeRotation.Yaw = 0;
		break;
	}


	//UE_LOG(LogTemp, Error, TEXT("Speed total: %s positive: %s negative: %s, value: %f, enum: %d"), *totalSpeed.ToString(), *positiveSpeed.ToString(), *negativeSpeed.ToString(), Command.Value, Command.Command);

	totalSpeed = positiveSpeed - negativeSpeed;
	totalRotation = positiveRotation - negativeRotation;
}

