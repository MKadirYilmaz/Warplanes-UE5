// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/BombComponent.h"
#include "PlaneScripts/BodyComponent.h"

UBombComponent::UBombComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBombComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBombComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (HasDropped)
	{
		LaunchVelocity += FVector(0, 0, -GRAVITY_VALUE * DeltaTime);

		FHitResult SurfaceHitResult;
		AddWorldOffset(LaunchVelocity, true, &SurfaceHitResult);
		if (SurfaceHitResult.GetActor()) // Bomb hitted an actor
		{
			FCollisionShape _Sphere = FCollisionShape::MakeSphere(ExplosionPower);
			FHitResult ExplosionResult;
			bool _InExplosionZone = GetWorld()->SweepSingleByChannel(ExplosionResult, SurfaceHitResult.ImpactPoint, SurfaceHitResult.ImpactPoint, FQuat::Identity, ECollisionChannel::ECC_Visibility, _Sphere);
			
			ExplodeBomb(SurfaceHitResult.GetComponent());
		}
	}
}

void UBombComponent::ExplodeBomb(UPrimitiveComponent* HitComp)
{
	UBodyComponent* _HittedBody = Cast<UBodyComponent>(HitComp);
	if (_HittedBody)
	{
		_HittedBody->SetTotalDurability(_HittedBody->GetCurrentDurability() - ExplosionPower);
	}
}
