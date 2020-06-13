// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Movements.generated.h"

/**
 * 
 */
UENUM()
enum class EMovements : uint8 {
	TUp UMETA(DisplayName = "Translate Up"),
	TUpStop UMETA(DisplayName = "Translate Up Stop"),
	TDown UMETA(DisplayName = "Translate Down"), 
	TDownStop UMETA(DisplayName = "Translate Down Stop"), 
	TLeft UMETA(DisplayName = "Translate Left"), 
	TLeftStop UMETA(DisplayName = "Translate Left Stop"), 
	TRight UMETA(DisplayName = "Translate Right"), 
	TRightStop UMETA(DisplayName = "Translate Right Stop"), 
	TForward UMETA(DisplayName = "Translate Forward"), 
	TForwardStop UMETA(DisplayName = "Translate Forward Stop"), 
	TBackward UMETA(DisplayName = "Translate Backward"), 
	TBackwardStop UMETA(DisplayName = "Translate Backward Stop"), 

	RLeft UMETA(DisplayName = "Rotate Left"),
	RLeftStop UMETA(DisplayName = "Rotate Left Stop"),
	RRight UMETA(DisplayName = "Rotate Right"),
	RRightStop UMETA(DisplayName = "Rotate Right Stop"),


	StopAll UMETA(DisplayName = "Stop All Movements"),
};