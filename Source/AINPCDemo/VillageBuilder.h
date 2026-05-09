#pragma once

#include "CoreMinimal.h"

class VillageBuilder
{
public:
	static void BuildVillage(UWorld* World);

private:
	static AActor* SpawnBox(UWorld* World, const FString& Name, FVector Location, FVector Scale, FLinearColor Color);
	static AActor* SpawnCylinder(UWorld* World, const FString& Name, FVector Location, FVector Scale, FLinearColor Color);
};
