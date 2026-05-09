#include "VillageBuilder.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "UObject/ConstructorHelpers.h"

void VillageBuilder::BuildVillage(UWorld* World)
{
	if (!World) return;

	// Ground
	SpawnBox(World, TEXT("Ground"), FVector(0, 0, -10), FVector(40, 40, 0.2f), FLinearColor(0.35f, 0.28f, 0.18f));

	// Walls
	SpawnBox(World, TEXT("WallNorth"), FVector(0, -1500, 150), FVector(30, 0.4f, 3), FLinearColor(0.5f, 0.48f, 0.45f));
	SpawnBox(World, TEXT("WallSouth"), FVector(0, 1500, 150), FVector(30, 0.4f, 3), FLinearColor(0.5f, 0.48f, 0.45f));
	SpawnBox(World, TEXT("WallEast"), FVector(1500, 0, 150), FVector(0.4f, 30, 3), FLinearColor(0.5f, 0.48f, 0.45f));
	SpawnBox(World, TEXT("WallWestTop"), FVector(-1500, -900, 150), FVector(0.4f, 12, 3), FLinearColor(0.5f, 0.48f, 0.45f));
	SpawnBox(World, TEXT("WallWestBot"), FVector(-1500, 900, 150), FVector(0.4f, 12, 3), FLinearColor(0.5f, 0.48f, 0.45f));

	// Market table
	SpawnBox(World, TEXT("MarketTable"), FVector(600, -500, 50), FVector(2.5f, 1.2f, 1), FLinearColor(0.55f, 0.35f, 0.15f));

	// Barrels
	SpawnCylinder(World, TEXT("Barrel1"), FVector(-500, 800, 40), FVector(0.8f, 0.8f, 0.8f), FLinearColor(0.45f, 0.3f, 0.12f));
	SpawnCylinder(World, TEXT("Barrel2"), FVector(-420, 850, 35), FVector(0.7f, 0.7f, 0.7f), FLinearColor(0.45f, 0.3f, 0.12f));

	// Quest board
	SpawnBox(World, TEXT("QuestBoard"), FVector(-700, -800, 120), FVector(1.2f, 0.15f, 1.8f), FLinearColor(0.45f, 0.3f, 0.15f));

	// Well
	SpawnCylinder(World, TEXT("Well"), FVector(0, 0, 40), FVector(2, 2, 0.8f), FLinearColor(0.45f, 0.43f, 0.4f));

	// Directional light
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = TEXT("SunLight");
	ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-45, 30, 0), SpawnParams);
	if (Sun)
	{
		Sun->GetLightComponent()->SetIntensity(3.0f);
		Sun->GetLightComponent()->SetLightColor(FLinearColor(1.0f, 0.95f, 0.85f));
	}

	// Sky light
	SpawnParams.Name = TEXT("SkyLight");
	ASkyLight* Sky = World->SpawnActor<ASkyLight>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (Sky)
	{
		Sky->GetLightComponent()->SetIntensity(1.0f);
	}
}

AActor* VillageBuilder::SpawnBox(UWorld* World, const FString& Name, FVector Location, FVector Scale, FLinearColor Color)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(*Name);

	AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, SpawnParams);
	if (Actor)
	{
		UStaticMesh* BoxMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
		Actor->GetStaticMeshComponent()->SetStaticMesh(BoxMesh);
		Actor->GetStaticMeshComponent()->SetWorldScale3D(Scale * 100.0f);

		UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(
			Actor->GetStaticMeshComponent()->GetMaterial(0), Actor);
		Mat->SetVectorParameterValue(TEXT("BaseColor"), Color);
		Actor->GetStaticMeshComponent()->SetMaterial(0, Mat);
	}
	return Actor;
}

AActor* VillageBuilder::SpawnCylinder(UWorld* World, const FString& Name, FVector Location, FVector Scale, FLinearColor Color)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(*Name);

	AStaticMeshActor* Actor = World->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator, SpawnParams);
	if (Actor)
	{
		UStaticMesh* CylMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder"));
		Actor->GetStaticMeshComponent()->SetStaticMesh(CylMesh);
		Actor->GetStaticMeshComponent()->SetWorldScale3D(Scale * 100.0f);

		UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(
			Actor->GetStaticMeshComponent()->GetMaterial(0), Actor);
		Mat->SetVectorParameterValue(TEXT("BaseColor"), Color);
		Actor->GetStaticMeshComponent()->SetMaterial(0, Mat);
	}
	return Actor;
}
