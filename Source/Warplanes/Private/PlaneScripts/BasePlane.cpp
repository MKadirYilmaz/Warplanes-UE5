// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/BasePlane.h"
#include "PlaneScripts/PlaneBodyComponent.h"
#include "PlaneScripts/GunComponent.h"
#include "PlaneScripts/PlaneMotorComponent.h"
#include "PlaneScripts/BombDropperComponent.h"


// Sets default values
ABasePlane::ABasePlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneBody = CreateDefaultSubobject<UPlaneBodyComponent>(TEXT("PlaneBody"));
	RootComponent = PlaneBody;
}

// Called when the game starts or when spawned
void ABasePlane::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> _ActorComps;
	_ActorComps = GetComponentsByTag(MotorClass, FName("Motor"));
	for (UActorComponent* _ActorComp : _ActorComps)
	{
		UPlaneMotorComponent* _MotorComp = Cast<UPlaneMotorComponent>(_ActorComp);
		if(_MotorComp)
			TiedMotors.Add(_MotorComp);
	}
	_ActorComps = GetComponentsByTag(GunClass, FName("Gun"));
	for (UActorComponent* _ActorComp : _ActorComps)
	{
		UGunComponent* _GunComp = Cast<UGunComponent>(_ActorComp);
		if (_GunComp)
			TiedGuns.Add(_GunComp);
	}

	RecalculateTotalMass();

}

// Called every frame
void ABasePlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RecalculateTotalMass();

	for (UPlaneMotorComponent* _Motor : TiedMotors)
	{
		if (!_Motor)
			continue;
		_Motor->ConsumeFuel(EquippedFuelAmount);
	}
	MoveForward(DeltaTime);
}

// Called to bind functionality to input
void ABasePlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("RotateUp"), this, &ABasePlane::RotateUpwards);
	PlayerInputComponent->BindAxis(TEXT("RotateRight"), this, &ABasePlane::RotateRight);
	PlayerInputComponent->BindAxis(TEXT("ChangeEnginePower"), this, &ABasePlane::ChangeEnginePower);
}

void ABasePlane::RotateUpwards(float Value)
{
	PlaneBody->AddLocalRotation(FRotator(PlaneBody->GetTurningSpeedMultiplier() / TotalMass, 0, 0));
}

void ABasePlane::RotateRight(float Value)
{
	PlaneBody->AddLocalRotation(FRotator(PlaneBody->GetRollRotationMultiplier() / TotalMass, 0, 0));
}

void ABasePlane::ChangeEnginePower(float Value)
{
	for (UPlaneMotorComponent* _Motor : TiedMotors)
	{
		if (!_Motor)
			continue;
		_Motor->AddCurrentEnginePower(Value);
	}
}

void ABasePlane::MoveForward(float DeltaTime)
{
	float _EnginePowers = GetPlaneTotalEnginePower();
	PlaneBody->AddWorldOffset(PlaneBody->GetForwardVector() * _EnginePowers * DeltaTime);
}

void ABasePlane::RotatePlaneToBalance()
{
	PlaneBody->AddLocalRotation(FRotator(0, 0, (TotalRightEnginePower - TotalLeftEnginePower) * EngineForceBalanceFactor / 10.f));
}

void ABasePlane::RecalculateTotalMass()
{
	TotalMass = GetPlaneTotalMass();
}

float ABasePlane::GetPlaneTotalMass()
{
	float _Mass = EquippedFuelAmount;
	_Mass += PlaneBody->GetRealMass(); // Add plane body mass to total mass
	for (UPlaneMotorComponent* _Motor : TiedMotors) // Add motor masses to total mass
	{
		if (!_Motor)
			continue;
		_Mass += _Motor->GetMass();
	}
	for (UGunComponent* _Gun : TiedGuns) // Add gun masses to total mass
	{
		if (!_Gun)
			continue;
		_Mass += _Gun->GetMass();
	}
	if (TiedBombDropper) // Add bomb dropper and all tied bombs masses to total mass
	{
		_Mass += TiedBombDropper->GetRealMass() + TiedBombDropper->GetTotalEquippedBombMass();
	}

	return _Mass;
}

float ABasePlane::GetPlaneTotalEnginePower()
{

	TotalRightEnginePower = 0.f;
	TotalLeftEnginePower = 0.f;
	MidEnginePower = 0.f;

	for (UPlaneMotorComponent* _Motor : TiedMotors)
	{
		if (!_Motor)
			continue;
		
		switch (_Motor->GetMotorPosition())
		{
		case EMotorPosition::RightSide:
			TotalRightEnginePower += _Motor->GetCurrentEnginePower();
			break;
		case EMotorPosition::LeftSide:
			TotalLeftEnginePower += _Motor->GetCurrentEnginePower();
			break;
		case EMotorPosition::MidSide:
			MidEnginePower += _Motor->GetCurrentEnginePower();
			break;
		default:
			break;

		}
	}
	return TotalRightEnginePower + TotalLeftEnginePower + MidEnginePower;
}



