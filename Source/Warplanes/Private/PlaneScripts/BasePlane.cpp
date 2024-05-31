// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/BasePlane.h"
#include "PlaneScripts/PlaneBodyComponent.h"
#include "PlaneScripts/GunComponent.h"
#include "PlaneScripts/PlaneMotorComponent.h"
#include "PlaneScripts/BombDropperComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABasePlane::ABasePlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlaneBody = CreateDefaultSubobject<UPlaneBodyComponent>(TEXT("PlaneBody"));
	RootComponent = PlaneBody;

	CockpitCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CockpitCamera"));
	CockpitCamera->SetupAttachment(RootComponent);
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
	DeltaSeconds = DeltaTime;

	RecalculateTotalMass();

	for (UPlaneMotorComponent* _Motor : TiedMotors)
	{
		if (!_Motor)
			continue;
		_Motor->ConsumeFuel(EquippedFuelAmount);
	}
	MoveForward(DeltaTime); // Plane position
	PlaneBody->AddLocalRotation(FRotator(DeltaPitch, 0, DeltaRoll)); // Plane rotation
	if(!CockpitCameraLock) // Player camera control
		CockpitCamera->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, CockpitCamera->GetRelativeRotation().Roll));
}

// Called to bind functionality to input
void ABasePlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("RotateUp"), this, &ABasePlane::RotateUpwards);
	PlayerInputComponent->BindAxis(TEXT("RotateRight"), this, &ABasePlane::RotateRight);
	PlayerInputComponent->BindAxis(TEXT("ChangeEnginePower"), this, &ABasePlane::ChangeEnginePower);

	PlayerInputComponent->BindAxis(TEXT("CameraMouseUp"), this, &ABasePlane::CameraUp);
	PlayerInputComponent->BindAxis(TEXT("CameraMouseRight"), this, &ABasePlane::CameraRight);

}

void ABasePlane::RotateUpwards(float Value)
{
	DeltaPitch = (PlaneBody->GetTurningSpeedMultiplier() * Value * DeltaSeconds * 1000.f) / TotalMass;
}

void ABasePlane::RotateRight(float Value)
{
	DeltaRoll = (PlaneBody->GetRollRotationMultiplier() * Value * DeltaSeconds * 1000.f) / TotalMass;
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

void ABasePlane::CameraUp(float Value)
{
	CameraPitch += Value;
	CameraPitch = FMath::Clamp(CameraPitch, -CameraRotationClamp.Pitch, CameraRotationClamp.Pitch);
}

void ABasePlane::CameraRight(float Value)
{
	CameraYaw += Value;
	CameraYaw = FMath::Clamp(CameraYaw, -CameraRotationClamp.Yaw, CameraRotationClamp.Yaw);
}

void ABasePlane::MoveForward(float DeltaTime)
{
	FVector _TargetForce = PlaneBody->GetForwardVector() * GetPlaneTotalEnginePower();

	// If the plane is slowing down then _InterpSpeed will speed up the slowing process. Otherwise it will make harder the plane to speed up.
	float _InterpSpeed = (_TargetForce.Length() < ForwardForce.Length()) ? PlaneBody->GetSurfaceResistanceMultiplier() * PlaneSlowMultiplier :
		1 / PlaneBody->GetSurfaceResistanceMultiplier() * PlaneAccelerationMultiplier;

	ForwardForce = FMath::VInterpTo(ForwardForce, _TargetForce, DeltaTime, _InterpSpeed);
	
	FVector _UpliftingForce = FVector(0, 0, FMath::Pow(ForwardForce.Length(), 2) - TotalMass * 10.f);
	_UpliftingForce.Z = FMath::Clamp(_UpliftingForce.Z, -1000.f, 10.f);

	PlaneBody->AddWorldOffset((ForwardForce + _UpliftingForce) * DeltaTime);
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
		_Mass += _Motor->GetRealMass();
	}
	for (UGunComponent* _Gun : TiedGuns) // Add gun masses to total mass
	{
		if (!_Gun)
			continue;
		_Mass += _Gun->GetRealMass();
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



