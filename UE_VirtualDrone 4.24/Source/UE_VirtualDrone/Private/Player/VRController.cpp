#include "VRController.h"
#include "ConstructorHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HandAnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "MotionControllerComponent.h"
#include "WidgetInteractionComponent.h"
#include "DrawDebugHelpers.h"

const float AVRController::InteractionDistance = 5000;

AVRController::AVRController()
{
	PrimaryActorTick.bCanEverTick = true;

	//cast "UClass" and Asset path is Different Why? -> Because UAnimBlueprint Class will crash a packaged game. so use postfix "_C", animation blueprint cast to UClass.
	static ConstructorHelpers::FObjectFinder<UClass> AnimBP_HandAnimation(TEXT("Class'/Game/Hand/Animations/ABP_RightHand.ABP_RightHand_C'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Hand(TEXT("SkeletalMesh'/Game/Hand/Mesh/SM_Hand_Right.SM_Hand_Right'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> M_Translucent(TEXT("MaterialInstanceConstant'/Game/Materials/M_Translucent_Inst.M_Translucent_Inst'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller"));
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	ZeroSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZeroSphere"));

	if (SM_Sphere.Succeeded())
		ZeroSphere->SetStaticMesh(SM_Sphere.Object);
	if (M_Translucent.Succeeded())
		ZeroSphere->SetMaterial(0, M_Translucent.Object);
	ZeroSphere->SetRelativeScale3D(FVector::OneVector * 0.3);

	RootComponent = RootScene;

	MotionController->SetupAttachment(RootComponent);
	MotionController->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MotionController->SetCollisionProfileName(TEXT("BlockAll"));

	HandMesh->SetupAttachment(MotionController);
	HandMesh->SetRelativeLocation(FVector(-12.8f, -0.03f, -1.8f));
	HandMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	HandMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh->CastShadow = false;

	if (SK_Hand.Succeeded())
	{
		HandMesh->SetSkeletalMesh(SK_Hand.Object);
		HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		if (AnimBP_HandAnimation.Succeeded())
			HandMesh->SetAnimInstanceClass(AnimBP_HandAnimation.Object);
	}

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction"));
	WidgetInteraction->SetupAttachment(MotionController);
	WidgetInteraction->InteractionDistance = InteractionDistance;
	SetActivated(false);

	ZeroSphere->SetupAttachment(RootComponent);

	Hand = EControllerHand::Left;
}

void AVRController::OnConstruction(const FTransform& Transform)
{
	MotionController->SetTrackingSource(Hand);
	if (Hand == EControllerHand::Right) {
		WidgetInteraction->VirtualUserIndex = 1;
		SetActivated(true);
	}
}

void AVRController::BeginPlay()
{
	Super::BeginPlay();

	if (Hand == EControllerHand::Left)
		HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
}

void AVRController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHandAnimation();

	FVector DirectionVector = GetControllerLocation() - ZeroSphere->GetComponentLocation();
	DrawDebugLine(GetWorld(), ZeroSphere->GetComponentLocation(), GetControllerLocation(), FColor(0, 255, 0), false, -1, 0, 1);

	switch (Hand)
	{
	case EControllerHand::Right:
		if (DirectionVector.X > 0)
		{
			if (!bIsGoingForward)
				bIsGoingForward = true;

			if (bIsGoingBackward)
			{
				bIsGoingBackward = false;
				OnCommand.Broadcast(EMovements::TBackwardStop, 0);
			}

			OnCommand.Broadcast(EMovements::TForward, GetSpeedPercent(DirectionVector.X));
		}
		else
		{
			if (!bIsGoingBackward)
				bIsGoingBackward = true;

			if (bIsGoingForward)
			{
				bIsGoingForward = false;
				OnCommand.Broadcast(EMovements::TForwardStop, 0);
			}

			OnCommand.Broadcast(EMovements::TBackward, GetSpeedPercent(-DirectionVector.X));
		}

		if (DirectionVector.Y > 0)
		{
			if (!bIsGoingLeft)
				bIsGoingLeft = true;

			if (bIsGoingRight)
			{
				bIsGoingRight = false;
				OnCommand.Broadcast(EMovements::TLeftStop, 0);
			}

			OnCommand.Broadcast(EMovements::TRight, GetSpeedPercent(DirectionVector.Y));

		}
		else
		{
			if (!bIsGoingRight)
				bIsGoingRight = true;

			if (bIsGoingLeft)
			{
				bIsGoingLeft = false;
				OnCommand.Broadcast(EMovements::TRightStop, 0);
			}

			OnCommand.Broadcast(EMovements::TLeft, GetSpeedPercent(-DirectionVector.Y));
		}

		if (DirectionVector.Z > 0)
		{
			if (!bIsGoingUp)
				bIsGoingUp = true;

			if (bIsGoingDown)
			{
				bIsGoingDown = false;
				OnCommand.Broadcast(EMovements::TDownStop, 0);
			}

			OnCommand.Broadcast(EMovements::TUp, GetSpeedPercent(DirectionVector.Z));

		}
		else
		{
			if (!bIsGoingDown)
				bIsGoingDown = true;

			if (bIsGoingUp)
			{
				bIsGoingUp = false;
				OnCommand.Broadcast(EMovements::TUpStop, 0);
			}

			OnCommand.Broadcast(EMovements::TDown, GetSpeedPercent(-DirectionVector.Z));
		}
		break;
	case EControllerHand::Left:

		if (DirectionVector.Y > 0)
		{
			if (!bIsGoingLeft)
				bIsGoingLeft = true;

			if (bIsGoingRight)
			{
				bIsGoingRight = false;
				OnCommand.Broadcast(EMovements::RLeftStop, 0);
			}

			OnCommand.Broadcast(EMovements::RLeft, GetRotationPercent(DirectionVector.Y));

		}
		else
		{
			if (!bIsGoingRight)
				bIsGoingRight = true;

			if (bIsGoingLeft)
			{
				bIsGoingLeft = false;
				OnCommand.Broadcast(EMovements::RRightStop, 0);
			}

			OnCommand.Broadcast(EMovements::RRight, GetRotationPercent(-DirectionVector.Y));
		}
		break;
	}


}

float AVRController::GetSpeedPercent(float Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(10, 20), FVector2D(0, 1), Value) * 10;
}

float AVRController::GetRotationPercent(float Value)
{
	return FMath::GetMappedRangeValueClamped(FVector2D(5, 10), FVector2D(0, 1), Value);
}

void AVRController::UpdateHandAnimation()
{
	//TODO: Define current hand state

	UHandAnimInstance* HandAnimation = Cast<UHandAnimInstance>(HandMesh->GetAnimInstance());

	//TODO: Add hand state to anim instance
}

void AVRController::TriggerPressed()
{
	if (bIsActivated)
		WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
}

void AVRController::TriggerReleased()
{
	if (bIsActivated)
		WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void AVRController::GripPressed()
{

}

void AVRController::GripReleased()
{

}

void AVRController::SetActivated(bool state)
{
	if (!WidgetInteraction) return;

	bIsActivated = state;
	WidgetInteraction->bEnableHitTesting = state;
	WidgetInteraction->bShowDebug = state; //TODO: Remove for build and replace with a real line component
}

FVector AVRController::GetControllerLocation() const
{
	return MotionController->GetComponentLocation();
}

FRotator AVRController::GetControllerRotation() const
{
	return MotionController->GetComponentRotation();
}

void AVRController::SetZeroSpherePosition(FVector Offset)
{
	ZeroSphere->SetWorldLocation(Offset);
}


