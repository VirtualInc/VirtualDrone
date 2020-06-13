// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Menu.generated.h"


class UCalibrationUserWidget;
class UPauseUserWidget;
class UWidgetComponent;
UCLASS()
class UE_VIRTUALDRONE_API AMenu : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* PauseWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* CalibrationWidgetComponent;

public:
	AMenu();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
		UPauseUserWidget* PauseWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables")
		UCalibrationUserWidget* CalibrationWidget;
public:

	UFUNCTION(BlueprintCallable, Category = "MenuComponent")
		void Pause() const;

	UFUNCTION(BlueprintCallable, Category = "MenuComponent")
		void UnPause();

	UFUNCTION(BlueprintCallable, Category = "MenuComponent")
		void OpenCalibration();
	
	UFUNCTION(BlueprintCallable, Category = "MenuComponent")
		void CloseCalibration();
};
