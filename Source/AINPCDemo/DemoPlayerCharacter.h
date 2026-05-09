#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DemoPlayerCharacter.generated.h"

class ANPCActor;
class UCameraComponent;

UCLASS()
class AINPCDEMO_API ADemoPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADemoPlayerCharacter();

	void SetNearestNPC(ANPCActor* NPC);
	void ClearNearestNPC(ANPCActor* NPC);

	UPROPERTY(BlueprintReadOnly, Category = "NPC")
	ANPCActor* NearestNPC = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIsInDialogue = false;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	void OnInteract();
	void OnCloseDialogue();
};
