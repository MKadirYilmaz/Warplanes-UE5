// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePlane.generated.h"


class UPlaneMotorComponent;
class UGunComponent;
class UBombDropperComponent;
class UPlaneBodyComponent;
class UCameraComponent;
UCLASS()
class WARPLANES_API ABasePlane : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Controls scripts
	void RotateUpwards(float Value);
	void RotateRight(float Value);
	void ChangeEnginePower(float Value);

	void MoveForward(float DeltaTime);

	void RotatePlaneToBalance();
	void RecalculateTotalMass();

	float GetPlaneTotalMass();
	float GetPlaneTotalEnginePower();


	UPROPERTY(EditAnywhere, Category = "Plane Setup")
	UPlaneBodyComponent* PlaneBody;

	UPROPERTY(VisibleAnywhere, Category = "Plane Setup")
	TArray<UPlaneMotorComponent*> TiedMotors;

	UPROPERTY(VisibleAnywhere, Category = "Plane Setup")
	TArray<UGunComponent*> TiedGuns;

	UPROPERTY(VisibleAnywhere, Category = "Plane Setup")
	UBombDropperComponent* TiedBombDropper;

	UPROPERTY(EditAnywhere, Category = "Plane Movement Adjustments")
	// When right engines total power is not equal to left engines total power plane will rotate because of unbalanced forces.
	float EngineForceBalanceFactor = 1.f;

	UPROPERTY(EditAnywhere)
	// Equipped fuel mass
	float EquippedFuelAmount = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Component Class Setup")
	TSubclassOf<UPlaneMotorComponent> MotorClass;

	UPROPERTY(EditAnywhere, Category = "Component Class Setup")
	TSubclassOf<UGunComponent> GunClass;


	float TotalMass = 0.f;
	float TotalRightEnginePower, TotalLeftEnginePower, MidEnginePower;

	FTimerHandle MassRecalculator;

};
