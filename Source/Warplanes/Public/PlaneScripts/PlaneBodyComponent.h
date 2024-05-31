// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaneScripts/BodyComponent.h"
#include "PlaneBodyComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARPLANES_API UPlaneBodyComponent : public UBodyComponent
{
	GENERATED_BODY()
	
public:
	// Default Constructor 
	UPlaneBodyComponent();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere)
	float RollRotationSpeedMultiplier = 10.f;

	UPROPERTY(EditAnywhere)
	float TurningSpeedMultiplier = 5.f;

	UPROPERTY(EditAnywhere)
	float SurfaceResistanceMultiplier = 2.f;

public:

	FORCEINLINE float GetRollRotationMultiplier() const { return RollRotationSpeedMultiplier; }
	FORCEINLINE float GetTurningSpeedMultiplier() const { return TurningSpeedMultiplier; }
	FORCEINLINE float GetSurfaceResistanceMultiplier() const { return SurfaceResistanceMultiplier; }

};
