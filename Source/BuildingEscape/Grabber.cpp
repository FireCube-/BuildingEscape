// Copyright © Bálint Gyevnár 2016

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	BindInputController();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics component not found! Please add PhysicsHandleComponent to %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::BindInputController()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("Grab"), IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction(TEXT("Grab"), IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input component not found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	if (ActorHit && PhysicsHandle)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ActorHit->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		FPlayerViewpoint Viewpoint = GetPlayerViewpoint();
		PhysicsHandle->SetTargetLocation(Viewpoint.LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FPlayerViewpoint Viewpoint = GetPlayerViewpoint();
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		Viewpoint.PlayerViewpointLocation,
		Viewpoint.LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);
	return Hit;
}

FPlayerViewpoint UGrabber::GetPlayerViewpoint() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	FVector LineTraceEnd;
	FPlayerViewpoint Viewpoint;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	Viewpoint.PlayerViewpointLocation = PlayerViewpointLocation;
	Viewpoint.PlayerViewpointRotation = PlayerViewpointRotation;
	Viewpoint.LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
	return Viewpoint;
}
