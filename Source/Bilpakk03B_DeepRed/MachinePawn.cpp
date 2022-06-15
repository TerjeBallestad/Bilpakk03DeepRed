// Fill out your copyright notice in the Description page of Project Settings.


#include "MachinePawn.h"

#include "PlayfieldContainer.h"
#include "Kismet/GameplayStatics.h"

AMachinePawn::AMachinePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	UMeshComponent *root = CreateDefaultSubobject<UMeshComponent>(TEXT("Root"));
	SetRootComponent(root);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm2"));
	CameraArm->SetupAttachment(root);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm);
}

void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<ABilpakkGameState>(UGameplayStatics::GetGameState(GetWorld()));
	check(GameState);
	
}

void AMachinePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMachinePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	/*PlayerInputComponent->BindAxis(TEXT("LeftVertical"), this, &AVRCarpakPawn::UpdateVertical);
	PlayerInputComponent->BindAxis(TEXT("LeftHorizontal"), this, &AVRCarpakPawn::UpdateHorizontal);*/
	PlayerInputComponent->BindAction(TEXT("MoveLeft"), IE_Pressed,this, &AMachinePawn::MoveLeft);
	PlayerInputComponent->BindAction(TEXT("MoveRight"), IE_Pressed,this, &AMachinePawn::MoveRight);
	PlayerInputComponent->BindAction(TEXT("MoveUp"), IE_Pressed,this, &AMachinePawn::MoveUp);
	PlayerInputComponent->BindAction(TEXT("MoveDown"), IE_Pressed,this, &AMachinePawn::MoveDown);
	PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed,this, &AMachinePawn::RotateLeft);
	PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed,this, &AMachinePawn::RotateRight);
	PlayerInputComponent->BindAction(TEXT("FlipForward"), IE_Pressed,this, &AMachinePawn::FlipForward);
	PlayerInputComponent->BindAction(TEXT("FlipBackward"), IE_Pressed,this, &AMachinePawn::FlipBackward);
	PlayerInputComponent->BindAction(TEXT("PlacePackage"), IE_Pressed,this, &AMachinePawn::PlacePackage);
	PlayerInputComponent->BindAction(TEXT("PanLeft"), IE_Pressed,this, &AMachinePawn::PanLeft);
	PlayerInputComponent->BindAction(TEXT("PanRight"), IE_Pressed,this, &AMachinePawn::PanRight);
}

void AMachinePawn::PlacePackage()
{
	GameState->Playfield->PlaceActivePackage();
}

void AMachinePawn::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Moving left"))
	GameState->Playfield->MovePreviewBlock(FIntVector(-1,0,0));
}

void AMachinePawn::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Moving right"))
	GameState->Playfield->MovePreviewBlock(FIntVector(1,0,0));
}

void AMachinePawn::MoveUp()
{
	GameState->Playfield->MovePreviewBlock(FIntVector(0,-1,0));
	UE_LOG(LogTemp, Warning, TEXT("Moving up "))
}

void AMachinePawn::MoveDown()
{
	GameState->Playfield->MovePreviewBlock(FIntVector(0,1,0));
	UE_LOG(LogTemp, Warning, TEXT("Moving down "))
}

void AMachinePawn::RotateLeft()
{
	GameState->Playfield->RotatePreviewBlock(FRotator(0, -90, 0));
	UE_LOG(LogTemp, Warning, TEXT("Rotating left "))
}

void AMachinePawn::RotateRight()
{
	GameState->Playfield->RotatePreviewBlock(FRotator(0, 90, 0));
	UE_LOG(LogTemp, Warning, TEXT("Rotating right "))
}

void AMachinePawn::FlipForward()
{
	GameState->Playfield->RotatePreviewBlock(FRotator(0, 0, 90));
	UE_LOG(LogTemp, Warning, TEXT("Flipping forward "))
}

void AMachinePawn::FlipBackward()
{
	GameState->Playfield->RotatePreviewBlock(FRotator(0, 0, -90));
	UE_LOG(LogTemp, Warning, TEXT("Flipping backward "))
}

void AMachinePawn::PanLeft()
{
	GameState->Playfield->PanPlayfield(FRotator(0,-90,0));
}

void AMachinePawn::PanRight()
{
	GameState->Playfield->PanPlayfield(FRotator(0,90,0));
}

