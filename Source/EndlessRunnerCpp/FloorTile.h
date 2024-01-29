// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Pickup.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFloorTile);

UCLASS()
class ENDLESSRUNNERCPP_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();

	UFUNCTION()
	FTransform GetAttachTransform();

	UPROPERTY(BlueprintAssignable)
	FFloorTile OnTileExited;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category=Tile)
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, Category=Tile)
	UBoxComponent* ExitTrigger;

	UPROPERTY(VisibleAnywhere, Category=Obstacle)
	UBoxComponent* ObstacleArea;

	UPROPERTY(EditAnywhere, Category=Obstacle)
	TArray<TSubclassOf<AObstacle>> ObstacleClasses;

	UPROPERTY(VisibleAnywhere, Category=Obstacle)
	float ObstacleSpawnChance;

	UPROPERTY(VisibleAnywhere, Category=Pickup)
	UBoxComponent* PickupArea;

	UPROPERTY(EditAnywhere, Category=Pickup)
	TArray<TSubclassOf<APickup>> PickupClasses;
	
	UPROPERTY(VisibleAnywhere, Category=Pickup)
	float PickupSpawnChance;

protected:
	UFUNCTION()
	FVector RandomPointInBoundingBox(UBoxComponent* Area);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnObstacles();

	UFUNCTION()
	void SpawnPickup();
	
	UFUNCTION()
	void DestroyActor();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
