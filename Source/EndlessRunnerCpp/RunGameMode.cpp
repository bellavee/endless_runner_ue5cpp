// Fill out your copyright notice in the Description page of Project Settings.


#include "RunGameMode.h"

#include "RunCharacter.h"
#include "Kismet/GameplayStatics.h"

ARunGameMode::ARunGameMode()
{
	InitTileNumber = 3;
}

void ARunGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < InitTileNumber; i++)
	{
		AddFloorTile();
	}
	
	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		RunCharacter->OnDeath.AddDynamic(this, &ARunGameMode::RestartLevel);
	}
}

void ARunGameMode::AddFloorTile()
{
	if (!TileClass) return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.bNoFail = true;
	SpawnParameters.Owner = this;

	AFloorTile* Tile = GetWorld()->SpawnActor<AFloorTile>(TileClass.Get(), NextAttachPoint, SpawnParameters);
	Tile->OnTileExited.AddDynamic(this, &ARunGameMode::AddFloorTile);
	
	NextAttachPoint = Tile->GetAttachTransform();
}

void ARunGameMode::RestartLevel()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("RestartLevel");
}
