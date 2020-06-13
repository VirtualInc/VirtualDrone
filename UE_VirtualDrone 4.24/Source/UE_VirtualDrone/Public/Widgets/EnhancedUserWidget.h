// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedUserWidget.generated.h"

/**
 *
 */
UCLASS()
class UE_VIRTUALDRONE_API UEnhancedUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		UWidgetAnimation* GetAnimationByName(FName AnimationName) const;
	/**
	 * Plays an animation in this widget a specified number of times
	 *
	 * @param AnimationName The animation to play
	 * @param StartAtTime The time in the animation from which to start playing, relative to the start position. For looped animations, this will only affect the first playback of the animation.
	 * @param NumLoopsToPlay The number of times to loop this animation (0 to loop indefinitely)
	 * @param PlayMode Specifies the playback mode
	 * @param PlaybackSpeed The speed at which the animation should play
	 */
	UFUNCTION()
		bool TryPlayAnimationByName(FName AnimationName, float StartAtTime = 0.0f, int32 NumLoopsToPlay = 1, EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward, float PlaybackSpeed = 1);

protected:

	UPROPERTY()
		TMap<FName, UWidgetAnimation*> AnimationsMap;
	UFUNCTION()
		void FillAnimationsMap();
	
};
