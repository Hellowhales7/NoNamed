// Fill out your copyright notice in the Description page of Project Settings.


#include "Flame.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
AFlame::AFlame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FiredFlame = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flame"));
	RootComponent = FiredFlame;
}

// Called when the game starts or when spawned
void AFlame::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

