// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

ARifle::ARifle()
{
	FireRate = 10.0f;
	FireTimer = 0.0f;
	bIsFiring = false;
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFiring && CurrentAmmo > 0)
	{
		FireTimer += DeltaTime;

		// 탄약 소모 로직 실행
		if (FireTimer >= 1.0f / FireRate)
		{
			SingleShot();
			FireTimer = 0.0f;
		}
	}
}

void ARifle::PullTrigger()
{
	if (CurrentAmmo <= 0)
	{
		UGameplayStatics::SpawnSoundAttached(GunEmpty, Mesh, TEXT("MuzzleFlashSocket"));
		return;
	}
	if (ShotMode == EShotMode::SM_Single)
	{
		SingleShot();
	}
	else if (ShotMode == EShotMode::SM_Auto)
	{
		bIsFiring=true;
	}
}

void ARifle::UnTrigger()
{
	bIsFiring = false;
}

bool ARifle::GunTrace(FHitResult& OutHit, FVector& OutShotDir)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector Loc;
	FRotator Roc;
	OwnerController->GetPlayerViewPoint(Loc, Roc);
	FVector Forward = OwnerController->GetPawn()->GetActorForwardVector();
	Loc = Loc + Forward * ForwardMul;
	//DrawDebugSphere(GetWorld(), Loc, 3, 12, FColor::Blue, true,5);
	FVector End = Loc + Roc.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	OutShotDir = -Roc.Vector();
	return GetWorld()->LineTraceSingleByChannel(OutHit, Loc, End, ECC_GameTraceChannel1, Params);;
}


void ARifle::ChangeShotMode()
{
	if (ShotMode == EShotMode::SM_Single)
		ShotMode = EShotMode::SM_Auto;
	else
		ShotMode = EShotMode::SM_Single;
	UE_LOG(LogTemp, Warning, TEXT("%d"), ShotMode);
}


void ARifle::SingleShot()
{
	CurrentAmmo--;
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult HitResult;
	FVector ShotDir;
	if (GunTrace(HitResult, ShotDir)) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDir.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
		if (HitResult.GetActor() != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *HitResult.GetActor()->GetName());
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDir, nullptr);
			AController* OwnerController = GetOwnerController();
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}
