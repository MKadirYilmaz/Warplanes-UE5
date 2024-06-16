// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class WARPLANES_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void OnBulletImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere)
	float BulletSlowSpeed = 1.f;

	FVector BulletVelocity = FVector(0, 0, 0);


public:
	
	FORCEINLINE FVector GetBulletVelocity() const { return BulletVelocity; }

	void SetBulletVelocity(FVector NewVelocity) { BulletVelocity = NewVelocity; }

};
