#include "VRPawn.h"
#include "VRController.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "UserWidget.h"
#include "CommandGameState.h"
#include "Menu.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	CameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	RootComponent = RootScene;
	CameraBase->SetupAttachment(RootComponent);
	VRCamera->SetupAttachment(CameraBase);
}

void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	const FName DeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	CameraBase->AddLocalOffset(FVector(0.0f, 0.0f, 20.f));

	const FTransform SpawnTransform = FTransform::Identity;
	const FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	UWorld* World = GetWorld();
	if (!World) return;

	GameState = (ACommandGameState*)World->GetGameState();

	MotionControllers.Add(World->SpawnActorDeferred<AVRController>(AVRController::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	MotionControllers.Add(World->SpawnActorDeferred<AVRController>(AVRController::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	if (MotionControllers[int32(EControllerHand::Left)])
	{
		MotionControllers[int32(EControllerHand::Left)]->Hand = EControllerHand::Left;
		MotionControllers[int32(EControllerHand::Left)]->FinishSpawning(SpawnTransform);
		MotionControllers[int32(EControllerHand::Left)]->AttachToComponent(CameraBase, AttachRules);
		MotionControllers[int32(EControllerHand::Left)]->OnCommand.AddDynamic(this, &AVRPawn::AddCommand);
	}

	if (MotionControllers[int32(EControllerHand::Right)])
	{
		MotionControllers[int32(EControllerHand::Right)]->Hand = EControllerHand::Right;
		MotionControllers[int32(EControllerHand::Right)]->FinishSpawning(SpawnTransform);
		MotionControllers[int32(EControllerHand::Right)]->AttachToComponent(CameraBase, AttachRules);
		MotionControllers[int32(EControllerHand::Right)]->OnCommand.AddDynamic(this, &AVRPawn::AddCommand);
	}

	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Menu = World->SpawnActor<AMenu>(AMenu::StaticClass(), SpawnTransform, Parameters);
	Menu->AttachToComponent(CameraBase, AttachRules);
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation = VRCamera->GetComponentLocation();
	CameraLocation.Z *= -1;

	if (MotionControllers[int32(EControllerHand::Left)])
		MotionControllers[int32(EControllerHand::Left)]->SetZeroSpherePosition(CameraLocation + LeftOffset);
	if (MotionControllers[int32(EControllerHand::Right)])
		MotionControllers[int32(EControllerHand::Right)]->SetZeroSpherePosition(CameraLocation + RightOffset);
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ResetVR"), IE_Pressed, this, &AVRPawn::OnResetVR);
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AVRPawn::OnPause);
	PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Pressed, this, &AVRPawn::OnTrigger<IE_Pressed, EControllerHand::Left>);
	PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Released, this, &AVRPawn::OnTrigger<IE_Released, EControllerHand::Left>);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Pressed, this, &AVRPawn::OnTrigger<IE_Pressed, EControllerHand::Right>);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Released, this, &AVRPawn::OnTrigger<IE_Released, EControllerHand::Right>);
	PlayerInputComponent->BindAction(TEXT("LeftGrip"), IE_Pressed, this, &AVRPawn::OnGrip<IE_Pressed, EControllerHand::Left>);
	PlayerInputComponent->BindAction(TEXT("LeftGrip"), IE_Released, this, &AVRPawn::OnGrip<IE_Released, EControllerHand::Left>);
	PlayerInputComponent->BindAction(TEXT("RightGrip"), IE_Pressed, this, &AVRPawn::OnGrip<IE_Pressed, EControllerHand::Right>);
	PlayerInputComponent->BindAction(TEXT("RightGrip"), IE_Released, this, &AVRPawn::OnGrip<IE_Released, EControllerHand::Right>);
	PlayerInputComponent->BindAction(TEXT("test"), IE_Pressed, this, &AVRPawn::Test);
}

void AVRPawn::Test()
{
	UE_LOG(LogTemp, Warning, TEXT("LeftHandPosition: x: %f, y:%f ,z:%f  LeftHandRotation x:%f,  y:%f, z:%f"),
		MotionControllers[int32(EControllerHand::Left)]->GetControllerLocation().X,
		MotionControllers[int32(EControllerHand::Left)]->GetControllerLocation().Y,
		MotionControllers[int32(EControllerHand::Left)]->GetControllerLocation().Z,
		MotionControllers[int32(EControllerHand::Left)]->GetControllerRotation().Roll,
		MotionControllers[int32(EControllerHand::Left)]->GetControllerRotation().Pitch,
		MotionControllers[int32(EControllerHand::Left)]->GetControllerRotation().Yaw
	);
	UE_LOG(LogTemp, Warning, TEXT("RightHandPosition: x: %f, y:%f ,z:%f  RightHandRotation x:%f,  y:%f, z:%f"),
		MotionControllers[int32(EControllerHand::Right)]->GetControllerLocation().X,
		MotionControllers[int32(EControllerHand::Right)]->GetControllerLocation().Y,
		MotionControllers[int32(EControllerHand::Right)]->GetControllerLocation().Z,
		MotionControllers[int32(EControllerHand::Right)]->GetControllerRotation().Roll,
		MotionControllers[int32(EControllerHand::Right)]->GetControllerRotation().Pitch,
		MotionControllers[int32(EControllerHand::Right)]->GetControllerRotation().Yaw
	);
}

void AVRPawn::AddCommand(EMovements Command, float Value)
{
	if (GameState)
		GameState->AddCommand(FCommand(Command, Value));
}

void AVRPawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVRPawn::OnPause()
{
	//TODO: Stop sending data to drone
	if (Menu)
		Menu->Pause();
}

float AVRPawn::GetCameraHeight()
{
	return VRCamera && CameraBase ? VRCamera->RelativeLocation.Z + CameraBase->RelativeLocation.Z + 15 : 0;
}

float AVRPawn::GetDistanceBetweenControllers()
{
	return
		MotionControllers[int32(EControllerHand::Left)] && MotionControllers[int32(EControllerHand::Right)] ?
		FVector::Distance(MotionControllers[int32(EControllerHand::Left)]->GetControllerLocation(), MotionControllers[int32(EControllerHand::Right)]->GetControllerLocation()) :
		0;
}

void AVRPawn::SetCalibrationData(const float BodyWidth, const float Height, const float ArmsLength)
{
	const float HalfHeight = Height / 2 + 50;
	const float ForearmLength = (ArmsLength - BodyWidth) / 2 - 30;
	const float HalfWidth = BodyWidth / 2;

	LeftOffset = FVector(ForearmLength, -HalfWidth, HalfHeight);
	RightOffset = FVector(ForearmLength, HalfWidth, HalfHeight);
}

template <EInputEvent InputEvent, EControllerHand Hand>
void AVRPawn::OnTrigger()
{
	const bool bIsLeftHandInput = Hand == EControllerHand::Left;
	MotionControllers[int32(EControllerHand::Left)]->SetActivated(bIsLeftHandInput);
	MotionControllers[int32(EControllerHand::Right)]->SetActivated(!bIsLeftHandInput);

	return
		InputEvent == IE_Pressed ? MotionControllers[int32(Hand)]->TriggerPressed() :
		InputEvent == IE_Released ? MotionControllers[int32(Hand)]->TriggerReleased() :
		void();
}

template <EInputEvent InputEvent, EControllerHand Hand>
void AVRPawn::OnGrip()
{
	if (IE_Released) amountOfGripPressed--;
	else if (IE_Pressed) amountOfGripPressed++;

	if (GameState)
		GameState->isAlive = amountOfGripPressed == 2;

	return
		InputEvent == IE_Pressed ? MotionControllers[int32(Hand)]->GripPressed() :
		InputEvent == IE_Released ? MotionControllers[int32(Hand)]->GripReleased() :
		void();
}

