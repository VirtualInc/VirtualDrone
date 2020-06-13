// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/EnhancedUserWidget.h"
#include "CalibrationUserWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class AVRPawn;

UENUM(BlueprintType)
enum class ECalibrationState : uint8
{
	CS_TPos 	UMETA(DisplayName = "TPos"),
	CS_IPos 	UMETA(DisplayName = "IPos"),
	CS_Done		UMETA(DisplayName = "Done"),
	CS_Failed	UMETA(DisplayName = "Failed")
};

USTRUCT(BlueprintType)
struct FCalibrationData
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<float> CameraHeights;

	UPROPERTY()
		TArray<float> DistancesBetweenControllers;

	void SetNewDistanceBetweenControllers(float Distance)
	{
		DistancesBetweenControllers.Add(Distance);
		DistancesBetweenControllers.RemoveAt(0);
	}

	void SetNewCameraHeight(float Height)
	{
		CameraHeights.Add(Height);
		CameraHeights.RemoveAt(0);
	}

	bool IsCalibrationReady() const
	{
		return !(DistancesBetweenControllers.Contains(0) || CameraHeights.Contains(0));
	}

	float GetDistanceBetweenControllersPrecision() const
	{
		return GetPrecision(DistancesBetweenControllers);
	}

	float GetAverageDistanceBetweenControllers() const
	{
		return GetAverage(DistancesBetweenControllers);
	}

	float GetCameraHeightPrecision() const
	{
		return GetPrecision(CameraHeights);
	}

	float GetAverageCameraHeight() const
	{
		return GetAverage(CameraHeights);
	}

	static float GetPrecision(TArray<float> Values)
	{
		const int AmountOfElements = Values.Num();
		float DispersionAmount = 0;

		for (int i = 0; i < AmountOfElements - 1; ++i)
			DispersionAmount += FMath::Abs(Values[i] - Values[i + 1]);

		return DispersionAmount / AmountOfElements * GetAverage(Values) / 100;
	}

	static float GetAverage(TArray<float> Values)
	{
		float Total = 0;
		for (const float Value : Values)
			Total += Value;
		return Total / Values.Num();
	}

	void Reset()
	{
		CameraHeights.Init(0, 10);
		DistancesBetweenControllers.Init(0, 10);
	}

	FCalibrationData()
	{
		Reset();
	}


	FString CameraToString() { return ToString(CameraHeights); }
	FString DistanceToString() { return ToString(DistancesBetweenControllers); }
	FString ToString(TArray<float> Values)
	{
		FString data;
		for (float Value : Values)
		{
			data += FString::SanitizeFloat(Value) + "\n";
		}
		return data;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCalibrationCloseClicked);
/**
 *
 */
UCLASS()
class UE_VIRTUALDRONE_API UCalibrationUserWidget : public UEnhancedUserWidget
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UButton* CloseButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UButton* CalibrationStartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UImage* TPosStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UImage* IPosStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UImage* CalibrationStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UImage* InstructionImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UTextBlock* Height;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UTextBlock* BodyWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UTextBlock* ArmsLength;

	


public:
	UCalibrationUserWidget();

	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void SetCurrentPawn(AVRPawn* PawnToCalibrate);

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void Calibrate();

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void ResetUI();

	UPROPERTY(BlueprintAssignable, Category = "Calibration|Event")
		FOnCalibrationCloseClicked OnCalibrationCloseClicked;

private:
	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void Close() { OnCalibrationCloseClicked.Broadcast(); }

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void SetStateImage(UTexture2D* TPosStateTexture, UTexture2D* IPosStateTexture, UTexture2D* CalibrationStateTexture);
	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void SetStateVisibility(ESlateVisibility TPosVisibility, ESlateVisibility IPosVisibility, ESlateVisibility CalibrationVisibility);

	UFUNCTION(BlueprintCallable, Category = "Calibration")
		void SetCalibrationState(ECalibrationState NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		AVRPawn* CurrentPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		uint32 bIsCalibrating : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		ECalibrationState CalibrationState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		float TimeSinceLastUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		FCalibrationData TPosData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		FCalibrationData IPosData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		UTexture2D* OrangeDot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		UTexture2D* RedCross;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Variables", meta = (AllowPrivateAccess = "true"))
		UTexture2D* GreenCheck;

	static const float TimeBetweenUpdates;
	static const float TimeBeforeFailSafe;
};
