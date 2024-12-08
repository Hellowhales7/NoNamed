// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Rifle.generated.h"

/**
 * 
 */


UCLASS()
class NONAMED_API ARifle : public AGun
{
	GENERATED_BODY()
public:
	ARifle();

	virtual void Tick(float DeltaTime) override;

	void PullTrigger() override;
	void UnTrigger() override;
	void ChangeShotMode() override;
protected:
	void SingleShot();

private:
	bool bIsFiring;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireTimer;
	UPROPERTY(EditAnywhere)
	float ForwardMul = 5;
	bool GunTrace(FHitResult& OutHit, FVector& OutShotDir);

};
