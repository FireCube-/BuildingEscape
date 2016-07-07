// Copyright © Bálint Gyevnár 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber initialised!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Player viewpoint location: %s"), *PlayerViewpointLocation.ToString())
	//UE_LOG(LogTemp, Warning, TEXT("Player viewpoint rotation: %s"), *PlayerViewpointRotation.ToString())

	DrawDebugLine(GetWorld(), PlayerViewpointLocation, PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach, FColor::Red, false, 0.f, 0.f, 10.f);
}

