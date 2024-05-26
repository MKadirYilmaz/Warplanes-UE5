// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BombDropperComponent.generated.h"

/**
 * 
 */
class UBombComponent;
UCLASS(Blueprintable)
class WARPLANES_API UBombDropperComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Default Constructor
	UBombDropperComponent();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void DropSingleBomb(int32 BombIndex, const FVector& LaunchVelocity);

	// This version doesn't wait for the BombLaunchDelayDuration. Drops all of the bombs at once
	void DropAllBombs(const FVector& LaunchVelocity);

	float GetTotalEquippedBombMass();

protected:
	UFUNCTION()
	void BombDropDelayFinished();	

	UPROPERTY(EditAnywhere)
	TArray<UBombComponent*> TiedBombs;

	UPROPERTY(EditAnywhere)
	// How long must pass to drop another bomb
	float BombLaunchDelayDuration = 0.5f;

	UPROPERTY(EditAnywhere)
	float RealMass = 50.f;

	FTimerHandle BombLaunchDelayer;

	bool CanDropAnotherBomb = true;
public:
	FORCEINLINE float GetRealMass() const { return RealMass; }
};
