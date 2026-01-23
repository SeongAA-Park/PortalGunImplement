// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/PortalGunShooterCharacter.h"

APortalGunShooterCharacter::APortalGunShooterCharacter()
{
	
}

void APortalGunShooterCharacter::AttachWeaponMeshes(AShooterWeapon* Weapon)
{
}

FVector APortalGunShooterCharacter::GetWeaponTargetLocation()
{
	return GetActorLocation();
}

void APortalGunShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APortalGunShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APortalGunShooterCharacter::InputShootBluePT()
{
}

void APortalGunShooterCharacter::InputShootOrangePT()
{
}

