#include "NPCActor.h"
#include "DemoPlayerCharacter.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

ANPCActor::ANPCActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root scene component
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Body capsule mesh
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		BodyMesh->SetStaticMesh(CylinderMesh.Object);
		BodyMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.0f));
	}

	// Emotion orb (sphere above head)
	EmotionOrb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmotionOrb"));
	EmotionOrb->SetupAttachment(Root);
	EmotionOrb->SetRelativeLocation(FVector(0, 0, 180));
	EmotionOrb->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh.Succeeded())
	{
		EmotionOrb->SetStaticMesh(SphereMesh.Object);
	}

	// Name label
	NameLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("NameLabel"));
	NameLabel->SetupAttachment(Root);
	NameLabel->SetRelativeLocation(FVector(0, 0, 140));
	NameLabel->SetHorizontalAlignment(EHTA_Center);
	NameLabel->SetVerticalAlignment(EVRTA_TextCenter);
	NameLabel->SetWorldSize(18.0f);
	NameLabel->SetTextRenderColor(FColor::White);

	// Interaction sphere
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(Root);
	InteractionSphere->SetSphereRadius(500.0f);
	InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	InteractionSphere->SetGenerateOverlapEvents(true);
}

void ANPCActor::BeginPlay()
{
	Super::BeginPlay();

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ANPCActor::OnInteractionBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ANPCActor::OnInteractionEndOverlap);

	// Apply body color
	UMaterialInstanceDynamic* BodyMat = UMaterialInstanceDynamic::Create(BodyMesh->GetMaterial(0), this);
	BodyMat->SetVectorParameterValue(TEXT("BaseColor"), NPCColor);
	BodyMesh->SetMaterial(0, BodyMat);

	// Set name label
	FString Label = FString::Printf(TEXT("%s\n%s"), *NPCName, *Role);
	NameLabel->SetText(FText::FromString(Label));

	UpdateEmotionOrb();
}

void ANPCActor::InitializeNPC(const FString& InName, const FString& InRole, FLinearColor InColor, const FString& InLocation)
{
	NPCName = InName;
	Role = InRole;
	NPCColor = InColor;
	NPCLocation = InLocation;
}

void ANPCActor::SetEmotion(const FString& Emotion, float Intensity)
{
	CurrentEmotion = Emotion.ToLower();
	EmotionIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
	UpdateEmotionOrb();
}

void ANPCActor::UpdateEmotionOrb()
{
	if (!EmotionOrb) return;

	FLinearColor Color = GetEmotionColor(CurrentEmotion);
	UMaterialInstanceDynamic* OrbMat = UMaterialInstanceDynamic::Create(EmotionOrb->GetMaterial(0), this);
	OrbMat->SetVectorParameterValue(TEXT("BaseColor"), Color);
	OrbMat->SetVectorParameterValue(TEXT("EmissiveColor"), Color * (0.5f + EmotionIntensity));
	EmotionOrb->SetMaterial(0, OrbMat);
}

FLinearColor ANPCActor::GetEmotionColor(const FString& Emotion) const
{
	if (Emotion == TEXT("happy")) return FLinearColor(1.0f, 0.9f, 0.2f);
	if (Emotion == TEXT("friendly")) return FLinearColor(1.0f, 0.85f, 0.3f);
	if (Emotion == TEXT("angry")) return FLinearColor(0.9f, 0.15f, 0.15f);
	if (Emotion == TEXT("sad")) return FLinearColor(0.2f, 0.3f, 0.9f);
	if (Emotion == TEXT("fearful")) return FLinearColor(0.6f, 0.2f, 0.8f);
	if (Emotion == TEXT("curious")) return FLinearColor(0.2f, 0.8f, 0.8f);
	if (Emotion == TEXT("suspicious")) return FLinearColor(0.9f, 0.5f, 0.1f);
	if (Emotion == TEXT("grateful")) return FLinearColor(0.2f, 0.8f, 0.3f);
	return FLinearColor(0.6f, 0.6f, 0.6f); // neutral
}

void ANPCActor::OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADemoPlayerCharacter* Player = Cast<ADemoPlayerCharacter>(OtherActor);
	if (Player)
	{
		Player->SetNearestNPC(this);
	}
}

void ANPCActor::OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADemoPlayerCharacter* Player = Cast<ADemoPlayerCharacter>(OtherActor);
	if (Player)
	{
		Player->ClearNearestNPC(this);
	}
}
