// Copyright © Bálint Gyevnár 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("Pressure plate missing on %s!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (GetTotalMassOnPlate() >= TriggerMass)
	{
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() > DoorLastOpenTime + DoorCloseDelay)
	{
		CloseDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}
}

float UOpenDoor::GetTotalMassOnPlate() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto& Actor : OverlappingActors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Overlapping actor: %s"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenDegree - 90.f, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, -90.f, 0.f));
}