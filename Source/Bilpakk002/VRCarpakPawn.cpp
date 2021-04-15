// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCarpakPawn.h"


#include "BilpakkGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AVRCarpakPawn::AVRCarpakPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	SetRootComponent(VRRoot);

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VRRoot);

}

void AVRCarpakPawn::SetupMap()
{
	AHandController* HandController = Cast<AHandController>(LeftHandController);
	if(HandController)
	{
		HandController->SetupMap();
	}
}

void AVRCarpakPawn::Setup()
{
	if(bSetup) return;
	
	RightHandController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);
	LeftHandController = GetWorld()->SpawnActor<AHandControllerBase>(HandControllerClass);

	if(RightHandController)
	{
		RightHandController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		RightHandController->SetOwner(this);
		RightHandController->SetHand(EControllerHand::Right);
	}
	if(LeftHandController)
	{
		LeftHandController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		LeftHandController->SetOwner(this);
		LeftHandController->SetHand(EControllerHand::Left);
	}

	bSetup = true;
}

void AVRCarpakPawn::BeginPlay()
{
	Super::BeginPlay();

	Setup();
	
}



void AVRCarpakPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	if(PlayerInputComponent)
	{
		/*PlayerInputComponent->BindAxis(TEXT("LeftVertical"), this, &AVRCarpakPawn::UpdateVertical);
		PlayerInputComponent->BindAxis(TEXT("LeftHorizontal"), this, &AVRCarpakPawn::UpdateHorizontal);*/
		PlayerInputComponent->BindAction(TEXT("RightGrip"), IE_Pressed,this, &AVRCarpakPawn::GripRight);
		PlayerInputComponent->BindAction(TEXT("RightGrip"), IE_Released,this, &AVRCarpakPawn::GripReleaseRight);
		PlayerInputComponent->BindAction(TEXT("LeftGrip"), IE_Pressed,this, &AVRCarpakPawn::GripLeft);
		PlayerInputComponent->BindAction(TEXT("LeftGrip"), IE_Released,this, &AVRCarpakPawn::GripReleaseLeft);
		PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Pressed,this, &AVRCarpakPawn::TriggerLeft);
		PlayerInputComponent->BindAction(TEXT("LeftTrigger"), IE_Released,this, &AVRCarpakPawn::TriggerReleaseLeft);
		PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Pressed,this, &AVRCarpakPawn::TriggerRight);
		PlayerInputComponent->BindAction(TEXT("RightTrigger"), IE_Released,this, &AVRCarpakPawn::TriggerReleaseRight);
		PlayerInputComponent->BindAction(TEXT("Pause"), IE_Released,this, &AVRCarpakPawn::PauseGame);
	}

}

void AVRCarpakPawn::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if(!LeftHandController) Setup();
	LeftHandController->SetActorHiddenInGame(bNewHidden);
	RightHandController->SetActorHiddenInGame(bNewHidden);
}

void AVRCarpakPawn::PauseGame_Implementation()
{
	GameState->TogglePause();
}

void AVRCarpakPawn::InitializeEvents_Implementation(ABilpakkGameState* State)
{
	GameState = State;
	GameState->OnNewGame.AddDynamic(this, &AVRCarpakPawn::NewGame);
	GameState->OnFinishedGame.AddDynamic(this, &AVRCarpakPawn::Finish);
	GameState->OnPausedGame.AddDynamic(this, &AVRCarpakPawn::Pause);
	GameState->OnUnPausedGame.AddDynamic(this, &AVRCarpakPawn::UnPause);
}


