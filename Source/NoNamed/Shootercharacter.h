// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Shootercharacter.generated.h"

class AGun;

UCLASS()
class NONAMED_API AShootercharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShootercharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//virtual float TakeDamage(
	//	float DamageAmount,
	//	struct FDamageEvent const& DamageEvent,
	//	class AController* EventInstigator,
	//	AActor* DamageCauser
	//) override;
	void Shot();
	void UnShot();

	UFUNCTION(BlueprintPure)
	AGun* GetGun();

	UFUNCTION(BlueprintCallable)
	void GetItem();
	void ChangeShotMode();
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUpRate(float Value);
	void LookRightRate(float Value);
	void GunReload();
	void ChangeToFirstWeapon();
	void ChangeToSecondWeapon();

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>> GunClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	AGun* Gun;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<AGun*> Guns;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnItem;

	UPROPERTY(EditDefaultsOnly)
	float  MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	bool Alive = true;
};
