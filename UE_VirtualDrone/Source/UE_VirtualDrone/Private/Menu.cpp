#include "Menu.h"
#include "ConstructorHelpers.h"
#include "WidgetComponent.h"
#include "CalibrationUserWidget.h"
#include "PauseUserWidget.h"

AMenu::AMenu()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> PauseWBP(TEXT("WidgetBlueprint'/Game/UI/WBP_Pause.WBP_Pause_C'"));
	static ConstructorHelpers::FObjectFinder<UClass> CalibrationWBP(TEXT("WidgetBlueprint'/Game/UI/WBP_Calibration.WBP_Calibration_C'"));
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	if (PauseWBP.Succeeded())
	{
		PauseWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Pause Menu"));
		PauseWidgetComponent->SetupAttachment(RootComponent);
		PauseWidgetComponent->SetWidgetClass(PauseWBP.Object);
		PauseWidgetComponent->SetRelativeLocation(FVector(175, 0, 90));
		PauseWidgetComponent->SetRelativeRotation(FRotator(0, 180, 0));
		PauseWidgetComponent->SetDrawSize(FVector2D( 180, 180));
	}

	if (CalibrationWBP.Succeeded())
	{
		CalibrationWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Calibration Menu"));
		CalibrationWidgetComponent->SetupAttachment(RootComponent);
		CalibrationWidgetComponent->SetWidgetClass(CalibrationWBP.Object);
		CalibrationWidgetComponent->SetRelativeLocation(FVector(150, 0, 90));
		CalibrationWidgetComponent->SetRelativeRotation(FRotator(0, 180, 0));
		CalibrationWidgetComponent->SetDrawSize(FVector2D(180, 180));
	}
}

void AMenu::BeginPlay()
{
	Super::BeginPlay();

	PauseWidget = Cast<UPauseUserWidget>(PauseWidgetComponent->GetUserWidgetObject());
	if (PauseWidget)
	{
		PauseWidget->OnCalibrationClicked.AddDynamic(this, &AMenu::OpenCalibration);
		PauseWidget->OnPlayClicked.AddDynamic(this, &AMenu::UnPause);
	}

	CalibrationWidget = Cast<UCalibrationUserWidget>(CalibrationWidgetComponent->GetUserWidgetObject());
	if (CalibrationWidget)
	{
		CalibrationWidget->OnCalibrationCloseClicked.AddDynamic(this, &AMenu::CloseCalibration);
	}

	UnPause();
	CloseCalibration();
	
	AVRPawn* Pawn = (AVRPawn*)GetOwner();
	if (Pawn && CalibrationWidget)
		CalibrationWidget->SetCurrentPawn(Pawn);
}

void AMenu::Pause() const
{
	if(PauseWidgetComponent)
		PauseWidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (PauseWidget)
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMenu::UnPause()
{
	if (PauseWidgetComponent)
		PauseWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (PauseWidget)
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AMenu::OpenCalibration()
{
	if (CalibrationWidgetComponent)
		CalibrationWidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (CalibrationWidget)
		CalibrationWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMenu::CloseCalibration()
{
	if (CalibrationWidgetComponent)
		CalibrationWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CalibrationWidget)
	{
		CalibrationWidget->SetVisibility(ESlateVisibility::Collapsed);
		CalibrationWidget->ResetUI();
	}
}

