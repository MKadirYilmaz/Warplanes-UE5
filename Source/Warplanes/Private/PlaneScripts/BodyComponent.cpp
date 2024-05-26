// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/BodyComponent.h"

UBodyComponent::UBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBodyComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UBodyComponent::BulletHit(const FHitResult& HitResult, float BulletSize, float BulletSpeed, const FVector& BulletDirection)
{
	if (CurrentDurability <= 0.f)
	{
		BodyExplosion();
		return;
	}
	float _DotValue = FVector::DotProduct(HitResult.ImpactNormal, BulletDirection);
	CurrentDurability -= BulletSize * FMath::Abs(_DotValue) * BulletSpeed;
}

void UBodyComponent::BodyExplosion()
{

}
