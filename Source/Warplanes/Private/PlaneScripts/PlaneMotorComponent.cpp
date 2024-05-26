// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/PlaneMotorComponent.h"

UPlaneMotorComponent::UPlaneMotorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	if (!GetOwner())
		return;
	
}

void UPlaneMotorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlaneMotorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//AddLocalRotation(FRotator(0, 0, CurrentEnginePower));
}

void UPlaneMotorComponent::ConsumeFuel(float& OutCurrentFuel)
{
	if (OutCurrentFuel <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Engine run out of fuel. Force shutdown!!"));
		CurrentEnginePower = 0.f;
		return;
	}
	else
	{
		OutCurrentFuel -= CurrentEnginePower / (FuelEfficiency * 100.f); // 100.f = additional efficiency
	}
}

void UPlaneMotorComponent::AddCurrentEnginePower(float AddValue)
{
	float _CurrentPercentage = CurrentEnginePower / MaxEnginePower * 100.f;
	_CurrentPercentage += AddValue;
	_CurrentPercentage = FMath::Clamp(_CurrentPercentage, 0.f, 100.f);

	CurrentEnginePower = MaxEnginePower * (_CurrentPercentage / 100.f);
}
