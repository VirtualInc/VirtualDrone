// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PauseUserWidget.h"
#include "Button.h"

bool UPauseUserWidget::Initialize()
{
	const bool result = Super::Initialize();

	if (PlayButton)
		PlayButton->OnClicked.AddDynamic(this, &UPauseUserWidget::PlayClicked);
	if (CalibrateButton)
		CalibrateButton->OnClicked.AddDynamic(this, &UPauseUserWidget::CalibrateClicked);

	return result;
}


