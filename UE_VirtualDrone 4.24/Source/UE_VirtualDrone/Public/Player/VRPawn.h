#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Movements.h"
#include "VRPawn.generated.h"

class AMenu;
class UCameraComponent;
class USceneComponent;
class AVRController;
class ACommandGameState;
UCLASS()
class UE_VIRTUALDRONE_API AVRPawn : public APawn
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* CameraBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* VRCamera;

public:
	AVRPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		void OnResetVR();

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		void OnPause();

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		float GetCameraHeight();

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		float GetDistanceBetweenControllers();

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		void SetCalibrationData(const float BodyWidth, const float Height, const float ArmsLength);

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		void Test();

	UFUNCTION(BlueprintCallable, Category = "VRPawn")
		void AddCommand(EMovements Command, float Value);

	template<EInputEvent InputEvent, EControllerHand Hand> void OnTrigger();
	template<EInputEvent InputEvent, EControllerHand Hand> void OnGrip();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		TArray <AVRController*> MotionControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		AMenu* Menu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		ACommandGameState* GameState;

	FVector LeftOffset;
	FVector RightOffset;

	uint8 amountOfGripPressed = 0;
};
