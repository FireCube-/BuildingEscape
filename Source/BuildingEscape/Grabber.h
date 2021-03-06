// Copyright © Bálint Gyevnár 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

struct FPlayerViewpoint
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	FVector LineTraceEnd;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:	
	//Sets reach used in calculation of line-trace endpoint
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//Find and initialise physics handler for interaction with objects
	void FindPhysicsHandleComponent();

	//Find and load on-the-fly-generated input controller into InputComponent
	void BindInputController();

	//Line-traces till an object with collision channel set to PhysicsBody is hit
	FHitResult GetFirstPhysicsBodyInReach() const;

	//Returns the direction of the player's viewpoint
	FPlayerViewpoint GetPlayerViewpoint() const;

	//Attaches a component to the physics handler for further interaction
	void Grab();

	//Detaches the previously attached object
	void Release();
};
