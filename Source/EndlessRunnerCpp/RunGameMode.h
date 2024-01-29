// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloorTile.h"
#include "GameFramework/GameModeBase.h"
#include "RunGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNERCPP_API ARunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARunGameMode();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void AddFloorTile();

	UFUNCTION()
	void RestartLevel();

	UPROPERTY(VisibleAnywhere)
	FTransform NextAttachPoint;

	UPROPERTY(VisibleAnywhere)
	int32 InitTileNumber;

	UPROPERTY(EditAnywhere)
	TAssetSubclassOf<AFloorTile> TileClass;
	
};
