// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/GunComponent.h"
#include "PlaneScripts/Bullet.h"
#include "Kismet/GameplayStatics.h"

UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UGunComponent::FireGun()
{
	GunAccuracy = FMath::Clamp(GunAccuracy, 0.f, 100.f);

	// Randomizes the direction based on GunAccuracy parameter
	FVector _Randomize = FVector(FMath::RandRange((GunAccuracy / 100.f) - 1.f, 1.f - (GunAccuracy / 100.f)), FMath::RandRange((GunAccuracy / 100.f) - 1.f, 1.f - (GunAccuracy / 100.f)), 
		FMath::RandRange((GunAccuracy / 100.f) - 1.f, 1.f - (GunAccuracy / 100.f)));

	// Adds the randomize vector to forward vector
	FVector _Direction = (GetForwardVector() + _Randomize).GetSafeNormal();
	// Final bullet speed and direction vector
	FVector _BulletForce = _Direction * GunFirePower;

	GetWorld()->SpawnActor<ABullet>(GetComponentLocation() + GetForwardVector() * 10.f, GetComponentRotation());
	if(GunFireSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunFireSound, GetComponentLocation());

}
