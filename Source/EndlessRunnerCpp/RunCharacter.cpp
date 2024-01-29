// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(-300.0f, 0.0f, 300.0f));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(Camera);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(RunCharacterMappingContext, 0);
		}	
	}

	IsDie = false;
}

void ARunCharacter::Move(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	if (!IsDie && Controller && (DirectionValue != 0.0f))
	{
		FVector Right = GetActorRightVector();
		AddMovementInput(Right, DirectionValue);
	}
}

void ARunCharacter::Die()
{
	FVector Location = GetMesh()->GetComponentLocation();
	FRotator Rotation = FRotator::ZeroRotator;
	FVector Scale = FVector(1.0f);
	
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Location, Rotation, Scale, true, EPSCPoolMethod::None, true);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, Location);
	}

	GetMesh()->SetVisibility(false);
	IsDie = true;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARunCharacter::CallOnDeath, 3.0f, false);
	
}

void ARunCharacter::CallOnDeath()
{
	this->OnDeath.Broadcast();
}

void ARunCharacter::AddCoin()
{
	TotalCoins += 1;
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), 1);
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ARunCharacter::Move);
	}
}

