// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	SetRootComponent(Root);
	RootComponent = Root;
	Mesh->SetupAttachment(Root);


}

void AGun::PullTrigger()
{

}

void AGun::UnTrigger()
{
}

void AGun::ChangeShotMode()
{
}

int32 AGun::GetAmmo()
{
	return CurrentAmmo;
}

int32 AGun::GetMaxAmmo()
{
	return MaxAmmo;
}

int32 AGun::GetMagazine()
{
	return AllAmmo;
}

FString AGun::GetShotMode()
{
	FString Mode;
	if (ShotMode == EShotMode::SM_Single)
		Mode=  TEXT("Single");
	else
		Mode= TEXT("Auto");
	return Mode;
}

void AGun::GetAmmoItem(int32 Ammo)
{
	AllAmmo += Ammo;
}

void AGun::Reload()
{
	UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));

	if (AllAmmo >= MaxAmmo) {
		AllAmmo -= MaxAmmo- CurrentAmmo;
		CurrentAmmo = MaxAmmo;
	}
	else {
		int32 ChargeAmmo = FMath::Min(AllAmmo, MaxAmmo - CurrentAmmo);
		CurrentAmmo += ChargeAmmo;
		AllAmmo -= ChargeAmmo;
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AGun::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn->GetController();
}

