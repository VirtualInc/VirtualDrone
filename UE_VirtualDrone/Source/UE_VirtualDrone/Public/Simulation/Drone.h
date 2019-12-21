#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

struct FCommand;
class USceneCaptureComponent2D;

UCLASS()
class UE_VIRTUALDRONE_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* Camera;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void ReceiveCommand(FCommand Command);

private:
	
	FVector positiveSpeed;
	FVector negativeSpeed;
	FVector totalSpeed;
};
