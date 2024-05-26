// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BodyComponent.generated.h"

UCLASS(Blueprintable)
class WARPLANES_API UBodyComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Default Constructor
	UBodyComponent();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BulletHit(const FHitResult& HitResult, float BulletSize, float BulletSpeed, const FVector& BulletDirection);

protected:

	void BodyExplosion();

	UPROPERTY(EditAnywhere)
	float ArmorThickness = 0.f;

	UPROPERTY(EditAnywhere)
	// Total health of the body before it explodes
	float TotalDurability = 0.f;

	UPROPERTY(EditAnywhere)
	float RealMass = 1.f;


	float CurrentDurability = TotalDurability;


public:
	FORCEINLINE void SetArmorThickness(float NewThickness) { ArmorThickness = NewThickness; }
	FORCEINLINE void SetTotalDurability(float NewTotalDurability) { TotalDurability = NewTotalDurability; }

	FORCEINLINE float GetRealMass() const { return RealMass; }
	FORCEINLINE float GetCurrentDurability() const { return CurrentDurability; }

};
