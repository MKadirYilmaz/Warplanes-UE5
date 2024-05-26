// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "PlaneMotorComponent.generated.h"

UENUM(BlueprintType)
enum EMotorPosition : uint8
{
	RightSide	UMETA(DisplayName = "RightSide"),
	LeftSide	UMETA(DisplayName = "LeftSide"),
	MidSide		UMETA(DisplayName = "MidSide")
};
UCLASS(Blueprintable)
class WARPLANES_API UPlaneMotorComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Default Constructor
	UPlaneMotorComponent();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ConsumeFuel(float& OutCurrentFuel);
	
	void AddCurrentEnginePower(float AddValue);
protected:
	UPROPERTY(EditAnywhere)
	// Position of the engine relative to the plane body
	TEnumAsByte<EMotorPosition> MotorPosition;

	UPROPERTY(EditAnywhere)
	// Base power of the engine
	float MaxEnginePower = 0.f;

	UPROPERTY(EditAnywhere)
	// Adds extra force to the engines. Because of that spin rate might drop, if the motor is spinning too fast
	float EnginePowerMultiplier = 1.f;

	UPROPERTY(EditAnywhere)
	// Determines how efficient the motor is. Higher values means lower fuel consumption
	float FuelEfficiency = 1.f;
	UPROPERTY(EditAnywhere)
	// Max motor health
	float MotorDurability = 100.f;

	float CurrentEnginePower = 0.f;
	float CurrentMotorDurability = MotorDurability;

public:

	FORCEINLINE void SetCurrentEnginePower(float NewPower) { CurrentEnginePower = NewPower; }
	FORCEINLINE void SetCurrentEngineDurability(float NewDurability) { CurrentMotorDurability = NewDurability; }

	FORCEINLINE float GetCurrentMotorDurability() const { return CurrentMotorDurability; }
	FORCEINLINE float GetCurrentEnginePower() const { return CurrentEnginePower; }
	FORCEINLINE EMotorPosition GetMotorPosition() const { return MotorPosition; }


	


};
