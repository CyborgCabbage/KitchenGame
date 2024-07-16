// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueData.h"

FDialogueData::FDialogueData() : FDialogueData(FText{}, 0.0f, true, false, nullptr)
{
}

FDialogueData::FDialogueData(FText Text, float WaitTime, bool LockPlayer, bool AutoAdvance, TObjectPtr<AActor> Target) :
	Text(Text),
	WaitTime(WaitTime),
	LockPlayer(LockPlayer),
	AutoAdvance(AutoAdvance),
	Target(Target)
{
}
