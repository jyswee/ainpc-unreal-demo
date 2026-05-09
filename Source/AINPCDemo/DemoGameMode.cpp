#include "DemoGameMode.h"
#include "DemoPlayerCharacter.h"
#include "NPCManager.h"
#include "NPCActor.h"
#include "VillageBuilder.h"

ADemoGameMode::ADemoGameMode()
{
	DefaultPawnClass = ADemoPlayerCharacter::StaticClass();
}

void ADemoGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Build the village environment
	VillageBuilder::BuildVillage(GetWorld());

	// Initialize NPC Manager
	NPCManagerInstance = NewObject<UNPCManager>(this);
	NPCManagerInstance->Initialize();

	// Wait for connection then spawn NPCs
	NPCManagerInstance->OnConnectionStatus.AddLambda([this](bool Connected)
	{
		if (Connected) SpawnNPCs();
	});
}

void ADemoGameMode::SpawnNPCs()
{
	ANPCActor* Elara = SpawnNPC(TEXT("Elara"), TEXT("merchant"),
		FLinearColor(0.85f, 0.65f, 0.13f), FVector(600, -300, 100), TEXT("market_stall"));

	ANPCActor* Theron = SpawnNPC(TEXT("Theron"), TEXT("guard"),
		FLinearColor(0.2f, 0.4f, 0.75f), FVector(-500, 600, 100), TEXT("village_gate"));

	ANPCActor* Miriam = SpawnNPC(TEXT("Old Miriam"), TEXT("quest_giver"),
		FLinearColor(0.2f, 0.7f, 0.3f), FVector(-700, -600, 100), TEXT("quest_board"));

	if (Elara) NPCManagerInstance->RegisterNPC(Elara);
	if (Theron) NPCManagerInstance->RegisterNPC(Theron);
	if (Miriam) NPCManagerInstance->RegisterNPC(Miriam);
}

ANPCActor* ADemoGameMode::SpawnNPC(const FString& Name, const FString& Role, FLinearColor Color, FVector Location, const FString& LocationName)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(*FString::Printf(TEXT("NPC_%s"), *Name));

	ANPCActor* NPC = GetWorld()->SpawnActor<ANPCActor>(Location, FRotator::ZeroRotator, SpawnParams);
	if (NPC)
	{
		NPC->InitializeNPC(Name, Role, Color, LocationName);
	}
	return NPC;
}
