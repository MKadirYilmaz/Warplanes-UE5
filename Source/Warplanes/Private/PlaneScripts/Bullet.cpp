// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));

	RootComponent = BulletMesh;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	BulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnBulletImpact);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(BulletVelocity);

	BulletVelocity -= (BulletVelocity.GetSafeNormal() * BulletSlowSpeed  + FVector(0, 0, 1)) * DeltaTime;
}

void ABullet::OnBulletImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("Bullet Impact"));
	Destroy();
}

