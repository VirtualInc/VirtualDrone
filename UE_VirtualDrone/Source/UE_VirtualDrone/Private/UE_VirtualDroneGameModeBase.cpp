#include "UE_VirtualDroneGameModeBase.h"
#include "VRPawn.h"
#include "CommandGameState.h"

AUE_VirtualDroneGameModeBase::AUE_VirtualDroneGameModeBase()
{
	DefaultPawnClass = AVRPawn::StaticClass();
	GameStateClass = ACommandGameState::StaticClass();
	
}
