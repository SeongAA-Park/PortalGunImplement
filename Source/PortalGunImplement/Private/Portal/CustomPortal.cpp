// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/CustomPortal.h"

#include "Components/BoxComponent.h"
#include "Portal/PortalGunShooterCharacter.h"

void ACustomPortal::CheckPlayerTeleport()
{
	TArray<AActor*> OverlappingPlayer;
	
	DetectBoxCollision->GetOverlappingActors(OverlappingPlayer, APortalGunShooterCharacter::StaticClass());
	
	if (OverlappingPlayer.IsEmpty()) { return; }
	
	
	
}
