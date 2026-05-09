#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameMode.generated.h"

class UNPCManager;
class ANPCActor;

UCLASS()
class AINPCDEMO_API ADemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADemoGameMode();

	virtual void BeginPlay() override;

	UPROPERTY()
	UNPCManager* NPCManagerInstance;

private:
	void SpawnNPCs();
	ANPCActor* SpawnNPC(const FString& Name, const FString& Role, FLinearColor Color, FVector Location, const FString& LocationName);
};
