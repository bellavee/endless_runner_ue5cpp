// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

#include "Pickup.h"
#include "RunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("Attach Point");
	AttachPoint->SetupAttachment(Scene);

	ExitTrigger = CreateDefaultSubobject<UBoxComponent>("Exit Trigger");
	ExitTrigger->SetupAttachment(Scene);

	ObstacleArea = CreateDefaultSubobject<UBoxComponent>("Obstacle Area");
	ObstacleArea->SetupAttachment(Scene);

	PickupArea = CreateDefaultSubobject<UBoxComponent>("Pickup Area");
	PickupArea->SetupAttachment(Scene);

	ObstacleSpawnChance = 0.6;
	PickupSpawnChance = 0.4;
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
	ExitTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnOverlapBegin);
	SpawnObstacles();
	for (int i = 0; i < 6; i++)
	{
		SpawnPickup();
	}
}

FTransform AFloorTile::GetAttachTransform()
{
	return AttachPoint->GetComponentTransform();
}

FVector AFloorTile::RandomPointInBoundingBox(UBoxComponent* Area)
{
	FVector BoxExtend = Area->GetScaledBoxExtent();
	FVector RandomPosition;
	
	RandomPosition.X = FMath::RandRange(-BoxExtend.X, BoxExtend.X);
	RandomPosition.Y = FMath::RandRange(-BoxExtend.Y, BoxExtend.Y);
	RandomPosition.Z = BoxExtend.Z;

	return RandomPosition;
}

void AFloorTile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		this->OnTileExited.Broadcast();
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFloorTile::DestroyActor, 0.5f, false);
	}
}

void AFloorTile::SpawnObstacles()
{
	FVector Location = RandomPointInBoundingBox(ObstacleArea) + ObstacleArea->GetRelativeLocation();
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	FTransform RelativeTransform = FTransform(Rotation, Location, Scale);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.bNoFail = true;
	spawnParams.Owner = this;

	
	if (ObstacleSpawnChance < FMath::RandRange(0.0f, 1.0f))
	{
		if (ObstacleClasses.Num() > 0)
		{
			int32 RandomIndex = FMath::RandHelper(ObstacleClasses.Num());
			
			AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(ObstacleClasses[RandomIndex].Get(), RelativeTransform, spawnParams);
			Obstacle->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Obstacle"));
			
		}
	}
}

void AFloorTile::SpawnPickup()
{
	FVector Location = RandomPointInBoundingBox(PickupArea) + ObstacleArea->GetRelativeLocation();
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	FTransform RelativeTransform = FTransform(Rotation, Location, Scale);
	
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParams.bNoFail = true;
	spawnParams.Owner = this;

	if (PickupSpawnChance < FMath::RandRange(0.0f, 1.0f))
	{
		if (PickupClasses.Num() > 0)
		{
			int32 RandomIndex = FMath::RandHelper(PickupClasses.Num());
			
			APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupClasses[RandomIndex].Get(), RelativeTransform, spawnParams);

			Pickup->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Pickup"));
			
		}
	}
	
}

void AFloorTile::DestroyActor()
{
	TArray<AActor*> temp;
	GetAttachedActors(temp);
	for (AActor* AttachedActor : temp)
	{
		AttachedActor->Destroy();
	}
 
	Destroy();
}


// Called every frame
void AFloorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


