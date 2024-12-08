// Fill out your copyright notice in the Description page of Project Settings.


#include "Shootercharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "NonamedGameMode.h"
#include "PickUpActor.h"
// Sets default values
AShootercharacter::AShootercharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShootercharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	for (int32 i = 0; i < GunClass.Num();i++) {
		Guns.Add(GetWorld()->SpawnActor<AGun>(GunClass[i]));
		Guns[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Guns[i]->SetOwner(this);
		Guns[i]->SetActorHiddenInGame(true);
	}
	if(Guns[0] != nullptr)
		Gun = Guns[0];
	Gun->SetActorHiddenInGame(false);
	//if (!GunClass.IsEmpty()) {
	//	Gun = GetWorld()->SpawnActor<AGun>(GunClass[0]);
	//	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	//	Gun->SetOwner(this);
	//}
	//else
	//	UE_LOG(LogTemp, Warning, TEXT("GunClass Empty"));
	Health = MaxHealth;
}

bool AShootercharacter::IsDead() const
{
	return Health <= 0;
}

float AShootercharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Called every frame
void AShootercharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShootercharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShootercharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShootercharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShootercharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRight"), this, &AShootercharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shot"), EInputEvent::IE_Pressed, this, &AShootercharacter::Shot);
	PlayerInputComponent->BindAction(TEXT("UnShot"), EInputEvent::IE_Released, this, &AShootercharacter::UnShot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShootercharacter::GunReload);
	PlayerInputComponent->BindAction(TEXT("FirstWeapon"), EInputEvent::IE_Pressed, this, &AShootercharacter::ChangeToFirstWeapon);
	PlayerInputComponent->BindAction(TEXT("SecondWeapon"), EInputEvent::IE_Pressed, this, &AShootercharacter::ChangeToSecondWeapon);
	PlayerInputComponent->BindAction(TEXT("ChangeShotMode"), EInputEvent::IE_Pressed, this, &AShootercharacter::ChangeShotMode);

}

float AShootercharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApplied = FMath::Min(Health, DamageToApplied);
	Health -= DamageToApplied;
	UE_LOG(LogTemp, Warning, TEXT("%f"), Health);
	if(IsDead())
	{
		if (Alive)
		{
			GetWorld()->SpawnActor<APickUpActor>(SpawnItem, GetActorLocation(), GetActorRotation());
			Alive = false;
		}
		ANonamedGameMode* NonamedGameMode = GetWorld()->GetAuthGameMode<ANonamedGameMode>();
		if (NonamedGameMode != nullptr)
		{
			NonamedGameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy(); // 컨트롤러를 때어냄
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageToApplied;
}

void AShootercharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AShootercharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AShootercharacter::Shot()
{
	if (Gun) {
		Gun->PullTrigger();
	}
}

void AShootercharacter::UnShot()
{
	if (Gun) {
		Gun->UnTrigger();
	}
}

AGun* AShootercharacter::GetGun()
{
	return Gun;
}

void AShootercharacter::GetItem()
{
	Gun->GetAmmoItem(10);
}

void AShootercharacter::ChangeShotMode()
{
	Gun->ChangeShotMode();
}

void AShootercharacter::LookUpRate(float Value)
{
	AddControllerPitchInput(Value*RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShootercharacter::LookRightRate(float Value)
{
	AddControllerYawInput(Value * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShootercharacter::GunReload()
{
	if (Gun->GetMagazine() > 0)
		Gun->Reload();
}

void AShootercharacter::ChangeToFirstWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon1"));
	if (!Guns[0] || Gun == Guns[0]) {
		UE_LOG(LogTemp, Warning, TEXT("No Weapon1"));
		return;
	}
	Gun->SetActorHiddenInGame(true);
	Gun = Guns[0];
	Gun->SetActorHiddenInGame(false);
}

void AShootercharacter::ChangeToSecondWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon2"));
	if (!Guns[1] || Gun == Guns[1]) {
		UE_LOG(LogTemp, Warning, TEXT("No Weapon2"));
		return;
	}
	Gun->SetActorHiddenInGame(true);
	Gun = Guns[1];
	Gun->SetActorHiddenInGame(false);
}


