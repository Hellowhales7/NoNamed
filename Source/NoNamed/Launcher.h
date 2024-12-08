// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Launcher.generated.h"
/**
 * 
 */
UCLASS()
class NONAMED_API ALauncher : public AGun
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ALauncher();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger() override;
	void UnTrigger() override;
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* ShotPoint;


	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* FiredFlame;
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* FlameCollision;

	bool bIsFiring;          
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate;        
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireTimer;

	void ConsumeAmmo(float DeltaTime);
	UFUNCTION()
	void OnOverlaped(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ApplyDamage(); 

	TSet<AActor*> OverlappingActors; 

	FTimerHandle DamageTimerHandle; 

};
