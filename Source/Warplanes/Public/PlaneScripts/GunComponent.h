// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GunComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class WARPLANES_API UGunComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
	// Default Constructor
	UGunComponent();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireGun();

protected:

	UPROPERTY(EditAnywhere)
	// Milimeter sized bullets
	float BulletSize = 7.68f;

	UPROPERTY(EditAnywhere)
	// The force of the gun. Determines how fast the bullet will go
	float GunFirePower = 10.f;

	UPROPERTY(EditAnywhere)
	// Higher value means less spreading
	float GunAccuracy = 75.f;

	UPROPERTY(EditAnywhere)
	// How many bullets will be fired in one second
	float FireRate = 5.f;

	UPROPERTY(EditAnywhere)
	float RealMass = 10.f;

	UPROPERTY(EditAnywhere)
	class USoundBase* GunFireSound;

public:
	FORCEINLINE float GetRealMass() const { return RealMass; }
	FORCEINLINE float GetFireRate() const { return FireRate; }


};
