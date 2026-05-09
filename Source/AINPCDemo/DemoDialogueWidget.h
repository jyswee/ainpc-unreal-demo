#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCEngineTypes.h"
#include "DemoDialogueWidget.generated.h"

class ANPCActor;
class UTextBlock;
class UEditableTextBox;
class UButton;
class UImage;
class UProgressBar;
class UScrollBox;

UCLASS()
class AINPCDEMO_API UDemoDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OpenForNPC(ANPCActor* NPC);
	void ClosePanel();
	void HandleResponse(ANPCActor* NPC, const FEventResult& Result);

	UPROPERTY(BlueprintReadOnly)
	ANPCActor* CurrentNPC = nullptr;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* NPCNameText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* NPCRoleText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* DialogueText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* EmotionText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UEditableTextBox* PlayerInput;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* SendButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* TradeButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* QuestButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* PortraitImage;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* MoodBar;

private:
	UFUNCTION()
	void OnSendClicked();

	UFUNCTION()
	void OnTradeClicked();

	UFUNCTION()
	void OnQuestClicked();

	UFUNCTION()
	void OnCloseClicked();

	UFUNCTION()
	void OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	FString DialogueHistory;

	void AppendDialogue(const FString& Text);
	void AppendSystemText(const FString& Text);
};
