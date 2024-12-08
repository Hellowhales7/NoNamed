// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"

ALauncher::ALauncher()
{
	ShotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShotPoint"));
	ShotPoint->SetupAttachment(Mesh);
	FiredFlame = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Flame"));
	FiredFlame->SetupAttachment(ShotPoint);
	FlameCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("FlameCollision"));
	FlameCollision->SetupAttachment(FiredFlame);
	FireRate = 10.0f;
	FireTimer = 0.0f;
	bIsFiring = false;
	ShotMode = EShotMode::SM_Auto;
}
void ALauncher::BeginPlay()
{
	Super::BeginPlay();
	FiredFlame->Deactivate();
	FlameCollision->SetGenerateOverlapEvents(false);
	FlameCollision->OnComponentBeginOverlap.AddDynamic(this, &ALauncher::OnOverlaped);
	FlameCollision->OnComponentEndOverlap.AddDynamic(this, &ALauncher::OnEndOverlap);

}


void ALauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFiring && CurrentAmmo > 0)
	{
		FireTimer += DeltaTime;

		// 탄약 소모 로직 실행
		if (FireTimer >= 1.0f / FireRate)
		{
			UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
			ConsumeAmmo(FireTimer);
			ApplyDamage();
			FireTimer = 0.0f; 
		}
	}
}

void ALauncher::PullTrigger()
{
	if (CurrentAmmo <= 0)
	{
		return;
	}
	//UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	//UE_LOG(LogTemp, Warning, TEXT("Weapon 2 shot"));
	bIsFiring = true;
	FiredFlame->Activate();
	FlameCollision->SetGenerateOverlapEvents(true);
}

void ALauncher::UnTrigger()
{
	if (!bIsFiring)
		return;
	bIsFiring = false;
	if (FiredFlame)
	{
		FiredFlame->Deactivate();
		FlameCollision->SetGenerateOverlapEvents(false);
	}
}

void ALauncher::ConsumeAmmo(float DeltaTime)
{
	CurrentAmmo -= FireRate * DeltaTime;
	if (CurrentAmmo <= 0)
	{
		CurrentAmmo = 0;
		UnTrigger(); // 탄약이 없으면 발사 중지
	}
}

void ALauncher::OnOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		OverlappingActors.Add(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s, Applied Damage: %.2f"), *OtherActor->GetName(), Damage);
	}
}

void ALauncher::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		OverlappingActors.Remove(OtherActor);
	}
}

void ALauncher::ApplyDamage()
{
	for (AActor* OverlapedActor : OverlappingActors)
	{
		if (OverlapedActor && OverlapedActor->IsValidLowLevel())
		{
			UGameplayStatics::ApplyDamage(
				OverlapedActor,              
				Damage,            
				GetInstigatorController(), 
				this,                   
				UDamageType::StaticClass() 
			);

		}
	}
}

