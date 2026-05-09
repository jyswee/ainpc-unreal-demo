#include "DemoPlayerCharacter.h"
#include "NPCActor.h"
#include "NPCManager.h"
#include "DemoGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ADemoPlayerCharacter::ADemoPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
	GetCapsuleComponent()->SetCapsuleRadius(35.0f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(RootComponent);
	CameraComp->SetRelativeLocation(FVector(0, 0, 60));
	CameraComp->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	bUseControllerRotationYaw = true;
}

void ADemoPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void ADemoPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADemoPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADemoPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADemoPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ADemoPlayerCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ADemoPlayerCharacter::OnInteract);
	PlayerInputComponent->BindAction(TEXT("CloseDialogue"), IE_Pressed, this, &ADemoPlayerCharacter::OnCloseDialogue);
}

void ADemoPlayerCharacter::MoveForward(float Value)
{
	if (bIsInDialogue) return;
	AddMovementInput(GetActorForwardVector(), Value);
}

void ADemoPlayerCharacter::MoveRight(float Value)
{
	if (bIsInDialogue) return;
	AddMovementInput(GetActorRightVector(), Value);
}

void ADemoPlayerCharacter::LookUp(float Value)
{
	if (bIsInDialogue) return;
	AddControllerPitchInput(Value);
}

void ADemoPlayerCharacter::LookRight(float Value)
{
	if (bIsInDialogue) return;
	AddControllerYawInput(Value);
}

void ADemoPlayerCharacter::OnInteract()
{
	if (bIsInDialogue || !NearestNPC) return;

	bIsInDialogue = true;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		PC->SetInputMode(InputMode);
	}

	ADemoGameMode* GM = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->NPCManagerInstance)
	{
		GM->NPCManagerInstance->ApproachNPC(NearestNPC);
	}
}

void ADemoPlayerCharacter::OnCloseDialogue()
{
	if (!bIsInDialogue) return;

	bIsInDialogue = false;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	ADemoGameMode* GM = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->NPCManagerInstance)
	{
		GM->NPCManagerInstance->LeaveNPC(NearestNPC);
	}
}

void ADemoPlayerCharacter::SetNearestNPC(ANPCActor* NPC)
{
	NearestNPC = NPC;
}

void ADemoPlayerCharacter::ClearNearestNPC(ANPCActor* NPC)
{
	if (NearestNPC == NPC)
		NearestNPC = nullptr;
}
