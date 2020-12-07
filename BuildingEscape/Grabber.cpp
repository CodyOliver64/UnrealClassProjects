// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include"Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Get Physics Handle Component
	FindPhysicsHandle();

	// Setup Inputs
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	// Attach Physics Handle(lives on the player) to the component that we grabbed
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Move components that are currently attached to our physics handle
	if (PhysicsHandle->GrabbedComponent)
	{
		// Continuously move the object that is currently attached to our PhysicsHandle to the LineTraceEnd which represents where we're currently lookng
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Ray Cast out a certain distance and return what was hit
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner()); // tagging, are we using complex tracing, what should we ignore (since our ray touches the player first)
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayersWorldPos(), GetPlayersReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	//                                   Stores Result             Start/End of line          Filters which collision channels we want to interact with       query specs

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation); //Viewpoint method takes in OUT parameters, these get updated instead of returning something

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

FVector UGrabber::GetPlayersWorldPos() const
{
	//Get Player Viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation); //Viewpoint method takes in OUT parameters, these get updated instead of returning something

	return PlayerViewPointLocation;
}