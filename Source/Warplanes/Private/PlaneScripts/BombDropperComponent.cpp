// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/BombDropperComponent.h"
#include "PlaneScripts/BombComponent.h"

UBombDropperComponent::UBombDropperComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBombDropperComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBombDropperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UBombDropperComponent::DropSingleBomb(int32 BombIndex, const FVector& LaunchVelocity)
{
	if (CanDropAnotherBomb && BombIndex < TiedBombs.Num() && TiedBombs[BombIndex] != nullptr)
	{
		TiedBombs[BombIndex]->SetLaunchVelocity(LaunchVelocity);
		TiedBombs[BombIndex]->SetHasDropped(true);
		CanDropAnotherBomb = false;

		GetOwner()->GetWorldTimerManager().SetTimer(BombLaunchDelayer, this, &UBombDropperComponent::BombDropDelayFinished, BombLaunchDelayDuration);
	}
}

void UBombDropperComponent::DropAllBombs(const FVector& LaunchVelocity)
{
	for (UBombComponent* _Bomb : TiedBombs)
	{
		if (_Bomb)
		{
			_Bomb->SetLaunchVelocity(LaunchVelocity);
			_Bomb->SetHasDropped(true);
		}
	}
}

void UBombDropperComponent::BombDropDelayFinished()
{
	CanDropAnotherBomb = true;
}

float UBombDropperComponent::GetTotalEquippedBombMass()
{
	float _Mass = 0.f;

	for (UBombComponent* _Bomb : TiedBombs)
	{
		if (_Bomb)
		{
			_Mass += _Bomb->GetRealMass();
		}
	}

	return _Mass;
}
