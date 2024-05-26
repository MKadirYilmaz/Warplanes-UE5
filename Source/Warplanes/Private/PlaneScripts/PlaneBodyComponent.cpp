// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneScripts/PlaneBodyComponent.h"

UPlaneBodyComponent::UPlaneBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlaneBodyComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlaneBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}
