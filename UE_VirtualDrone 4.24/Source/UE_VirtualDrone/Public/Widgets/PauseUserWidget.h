// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/EnhancedUserWidget.h"
#include "PauseUserWidget.generated.h"

class UCalibrationUserWidget;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCalibrationClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayClicked);
/**
 *
 */
UCLASS()
class UE_VIRTUALDRONE_API UPauseUserWidget : public UEnhancedUserWidget
{
	GENERATED_BODY()


		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UButton* PlayButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true", BindWidget))
		UButton* CalibrateButton;
	
public:
	virtual bool Initialize() override;

	UPROPERTY(BlueprintAssignable, Category = "Pause|Event")
		FOnPlayClicked OnPlayClicked;
	
	UPROPERTY(BlueprintAssignable, Category = "Pause|Event")
		FOnCalibrationClicked OnCalibrationClicked;

private:

	UFUNCTION(BlueprintCallable, Category = "Pause")
		void PlayClicked() { OnPlayClicked.Broadcast(); }
	
	UFUNCTION(BlueprintCallable, Category = "Pause")
		void CalibrateClicked() { OnCalibrationClicked.Broadcast(); }
};
