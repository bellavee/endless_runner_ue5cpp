// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "RunCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(Scene);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnGet);

	CanBePickUp = false;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup::OnGet(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		OnPlayerOverlap();
	}
}

void APickup::OnPlayerOverlap()
{
	if (IsPickedUp())
	{
		OnPickup();
	}
	
}

void APickup::OnPickup()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
	Destroy();
}

bool APickup::IsPickedUp()
{
	CanBePickUp = true;
	return CanBePickUp;
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

