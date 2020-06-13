// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CalibrationUserWidget.h"
#include "VRPawn.h"

#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Image.h"
#include "Button.h"
#include "TextBlock.h"

const float UCalibrationUserWidget::TimeBetweenUpdates = 300;
const float UCalibrationUserWidget::TimeBeforeFailSafe = 5000;

#define LOCTEXT_NAMESPACE "Body"

UCalibrationUserWidget::UCalibrationUserWidget()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> OrangeDotTexture(TEXT("Texture2D'/Game/Images/OrangeDot.OrangeDot'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> RedCrossTexture(TEXT("Texture2D'/Game/Images/RedCross.RedCross'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> GreenCheckTexture(TEXT("Texture2D'/Game/Images/GreenCheckMark.GreenCheckMark'"));

	if (OrangeDotTexture.Succeeded())
		OrangeDot = OrangeDotTexture.Object;
	if (RedCrossTexture.Succeeded())
		RedCross = RedCrossTexture.Object;
	if (GreenCheckTexture.Succeeded())
		GreenCheck = GreenCheckTexture.Object;

}

bool UCalibrationUserWidget::Initialize()
{
	const bool result = Super::Initialize();

	if (CloseButton)
		CloseButton->OnClicked.AddDynamic(this, &UCalibrationUserWidget::Close);
	if (CalibrationStartButton)
		CalibrationStartButton->OnClicked.AddDynamic(this, &UCalibrationUserWidget::Calibrate);

	if (TPosStateImage)
		TPosStateImage->SetVisibility(ESlateVisibility::Hidden);
	if (IPosStateImage)
		IPosStateImage->SetVisibility(ESlateVisibility::Hidden);
	if (CalibrationStateImage)
		CalibrationStateImage->SetVisibility(ESlateVisibility::Hidden);

	return result;
}

void UCalibrationUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsCalibrating && CurrentPawn)
	{
		TimeSinceLastUpdate += InDeltaTime * 1000;
		if (TimeSinceLastUpdate < TimeBetweenUpdates) return;
		if (TimeSinceLastUpdate > TimeBeforeFailSafe)
		{
			SetCalibrationState(ECalibrationState::CS_Failed);
			return;
		}

		TimeSinceLastUpdate = 0;

		switch (CalibrationState)
		{
		case ECalibrationState::CS_TPos:
			TPosData.SetNewCameraHeight(CurrentPawn->GetCameraHeight());
			TPosData.SetNewDistanceBetweenControllers(CurrentPawn->GetDistanceBetweenControllers());

			if (!TPosData.IsCalibrationReady()) return;

			if (TPosData.GetCameraHeightPrecision() < 5 && TPosData.GetDistanceBetweenControllersPrecision() < 5)
				SetCalibrationState(ECalibrationState::CS_IPos);

			break;
		case ECalibrationState::CS_IPos:
			IPosData.SetNewCameraHeight(CurrentPawn->GetCameraHeight());
			IPosData.SetNewDistanceBetweenControllers(CurrentPawn->GetDistanceBetweenControllers());

			if (!IPosData.IsCalibrationReady()) return;

			if (IPosData.GetCameraHeightPrecision() < 5 && IPosData.GetDistanceBetweenControllersPrecision() < 5)
				SetCalibrationState(ECalibrationState::CS_Done);
			break;
		}
	}
}

void UCalibrationUserWidget::SetCurrentPawn(AVRPawn* PawnToCalibrate)
{
	CurrentPawn = PawnToCalibrate;
}

void UCalibrationUserWidget::Calibrate()
{
	if (!bIsCalibrating && CurrentPawn)
		SetCalibrationState(ECalibrationState::CS_TPos);
}

void UCalibrationUserWidget::ResetUI()
{
	SetStateVisibility(ESlateVisibility::Hidden, ESlateVisibility::Hidden, ESlateVisibility::Hidden);
	//TODO: Change instruction image;
}

void UCalibrationUserWidget::SetStateImage(UTexture2D* TPosStateTexture, UTexture2D* IPosStateTexture, UTexture2D* CalibrationStateTexture)
{
	if (TPosStateTexture)
		TPosStateImage->SetBrushFromTexture(TPosStateTexture);
	if (IPosStateTexture)
		IPosStateImage->SetBrushFromTexture(IPosStateTexture);
	if (CalibrationStateTexture)
		CalibrationStateImage->SetBrushFromTexture(CalibrationStateTexture);
}

void UCalibrationUserWidget::SetStateVisibility(ESlateVisibility TPosVisibility, ESlateVisibility IPosVisibility, ESlateVisibility CalibrationVisibility)
{
	TPosStateImage->SetVisibility(TPosVisibility);
	IPosStateImage->SetVisibility(IPosVisibility);
	CalibrationStateImage->SetVisibility(CalibrationVisibility);
}

void UCalibrationUserWidget::SetCalibrationState(ECalibrationState NewState)
{
	//float BodyWidthValue, HeightValue, ArmsLengthValue;
	
	switch (NewState)
	{
	case ECalibrationState::CS_TPos:
		CalibrationStartButton->SetIsEnabled(false);
		SetStateImage(OrangeDot, nullptr, OrangeDot);
		SetStateVisibility(ESlateVisibility::SelfHitTestInvisible, ESlateVisibility::Hidden, ESlateVisibility::SelfHitTestInvisible);

		bIsCalibrating = true;
		TPosData.Reset();

		//TODO: show TPOS instruction

		break;
	case ECalibrationState::CS_IPos:
		SetStateImage(GreenCheck, OrangeDot, nullptr);
		SetStateVisibility(ESlateVisibility::SelfHitTestInvisible, ESlateVisibility::SelfHitTestInvisible, ESlateVisibility::SelfHitTestInvisible);

		IPosData.Reset();

		//TODO: show IPOS instruction

		break;
	case ECalibrationState::CS_Done:
	{
		CalibrationStartButton->SetIsEnabled(true);
		SetStateImage(nullptr, GreenCheck, GreenCheck);

		bIsCalibrating = false;


		const float BodyWidthValue = IPosData.GetAverageDistanceBetweenControllers();
		const float HeightValue = (TPosData.GetAverageCameraHeight() + IPosData.GetAverageCameraHeight()) / 2;
		const float ArmsLengthValue = TPosData.GetAverageDistanceBetweenControllers();

		BodyWidth->SetText(FText::Format(LOCTEXT("BodyWitdh", "Largeur d'épaules : {0} cm"), BodyWidthValue));
		Height->SetText(FText::Format(LOCTEXT("Height", "Taille : {0} cm"), HeightValue));
		ArmsLength->SetText(FText::Format(LOCTEXT("ArmsLength", "Longueur des bras : {0} cm"), ArmsLengthValue));

		if (CurrentPawn)
			CurrentPawn->SetCalibrationData(BodyWidthValue, HeightValue, ArmsLengthValue);

		//TODO: send calibration data to pawn and save
		break;
	}
	case ECalibrationState::CS_Failed:
		CalibrationStartButton->SetIsEnabled(true);
		if (CalibrationState == ECalibrationState::CS_TPos)
		{
			SetStateImage(RedCross, nullptr, RedCross);
			SetStateVisibility(ESlateVisibility::SelfHitTestInvisible, ESlateVisibility::Hidden, ESlateVisibility::SelfHitTestInvisible);
		}
		else if (CalibrationState == ECalibrationState::CS_IPos)
		{
			SetStateImage(nullptr, RedCross, RedCross);
			SetStateVisibility(ESlateVisibility::Hidden, ESlateVisibility::Hidden, ESlateVisibility::SelfHitTestInvisible);
		}
		break;
	}

	CalibrationState = NewState;
	TimeSinceLastUpdate = 0;
}



#undef LOCTEXT_NAMESPACE