// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"


UENUM() // 블루프린트에서도 사용할 수 있게 설정
enum class EShotMode : uint8
{
	SM_Single UMETA(DisplayName = "Single"),
	SM_Auto  UMETA(DisplayName = "Auto"),
	SM_End  UMETA(DisplayName = "End"),
};

UCLASS()
class NONAMED_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	virtual void PullTrigger();
	virtual void UnTrigger();
	virtual void ChangeShotMode();

	UFUNCTION(BlueprintPure)
	int32 GetAmmo();

	UFUNCTION(BlueprintPure)
	int32 GetMaxAmmo();

	UFUNCTION(BlueprintPure)
	int32 GetMagazine();

	UFUNCTION(BlueprintPure)
	virtual FString GetShotMode();

	void GetAmmoItem(int32 Ammo);
	void Reload();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 MaxAmmo = 30;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	int32 AllAmmo = 100;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	EShotMode ShotMode = EShotMode::SM_Single;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* GunEmpty;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ImpactSound;


	AController* GetOwnerController();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;


};
