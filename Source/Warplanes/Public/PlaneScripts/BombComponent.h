// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BombComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class WARPLANES_API UBombComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Default Constructor
	UBombComponent();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void ExplodeBomb(UPrimitiveComponent* HitComp);
protected:

	UPROPERTY(EditAnywhere)
	float ExplosionPower = 0.f;

	UPROPERTY(EditAnywhere)
	float RealMass = 50.f;

	UPROPERTY(EditAnywhere)
	float GRAVITY_VALUE = 98.f;

	FVector LaunchVelocity;

	bool HasDropped = false;


public:
	FORCEINLINE void SetLaunchVelocity(FVector NewVelocity) { LaunchVelocity = NewVelocity; }
	FORCEINLINE void SetHasDropped(bool bDropped) { HasDropped = bDropped; }

	FORCEINLINE float GetRealMass() const { return RealMass; }
};
