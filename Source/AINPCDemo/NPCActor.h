#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"
#include "NPCActor.generated.h"

UCLASS()
class AINPCDEMO_API ANPCActor : public AActor
{
	GENERATED_BODY()

public:
	ANPCActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString NPCName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString Role;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FLinearColor NPCColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FString NPCLocation;

	UPROPERTY(BlueprintReadOnly, Category = "NPC")
	FString ServerNPCId;

	UPROPERTY(BlueprintReadOnly, Category = "NPC")
	FString CurrentEmotion = TEXT("neutral");

	UPROPERTY(BlueprintReadOnly, Category = "NPC")
	float EmotionIntensity = 0.0f;

	void InitializeNPC(const FString& InName, const FString& InRole, FLinearColor InColor, const FString& InLocation);
	void SetEmotion(const FString& Emotion, float Intensity = 0.5f);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* EmotionOrb;

	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* NameLabel;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionSphere;

private:
	void UpdateEmotionOrb();
	FLinearColor GetEmotionColor(const FString& Emotion) const;

	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
