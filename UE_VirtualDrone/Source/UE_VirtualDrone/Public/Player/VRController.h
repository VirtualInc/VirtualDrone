#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRController.generated.h"


class UStaticMeshComponent;
class AVRPawn;
class UWidgetInteractionComponent;
class UMotionControllerComponent;
class USceneComponent;
class Command;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCommand, FString, CommandName, float, Value);

UCLASS()
class UE_VIRTUALDRONE_API AVRController : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UWidgetInteractionComponent* WidgetInteraction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ZeroSphere;

public:
	AVRController();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	float GetSpeedPercent(float Value);
	
	UFUNCTION(BlueprintCallable, Category = "VRController")
		void UpdateHandAnimation();

	UFUNCTION(BlueprintCallable, Category = "VRController")
		FORCEINLINE UMotionControllerComponent* GetMotionController() const { return MotionController; }

	UFUNCTION(BlueprintCallable, Category = "VRController")
		virtual void TriggerPressed();
	UFUNCTION(BlueprintCallable, Category = "VRController")
		virtual void TriggerReleased();
	UFUNCTION(BlueprintCallable, Category = "VRController")
		virtual void GripPressed();
	UFUNCTION(BlueprintCallable, Category = "VRController")
		virtual void GripReleased();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		EControllerHand Hand;

	UPROPERTY(BlueprintAssignable, Category = "VRController|Event")
		FOnCommand OnCommand;

	UFUNCTION(BlueprintCallable, Category = "VRController")
		FORCEINLINE	bool IsActivated() const { return bIsActivated; }
	UFUNCTION(BlueprintCallable, Category = "VRController")
		void SetActivated(bool state);

	UFUNCTION(BlueprintCallable, Category = "VRController")
		FVector GetControllerLocation() const;

	UFUNCTION(BlueprintCallable, Category = "VRController")
		FRotator GetControllerRotation() const;

	UFUNCTION(BlueprintCallable, Category = "VRController")
		void SetZeroSpherePosition(FVector Offset);

private:

	static const float InteractionDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32 bIsActivated : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32  bIsGoingForward : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32  bIsGoingBackward : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32  bIsGoingUp : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32  bIsGoingDown : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32  bIsGoingLeft : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32  bIsGoingRight : 1;
};
