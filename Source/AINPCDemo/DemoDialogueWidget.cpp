#include "DemoDialogueWidget.h"
#include "NPCActor.h"
#include "NPCManager.h"
#include "DemoGameMode.h"
#include "DemoPlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UDemoDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SendButton) SendButton->OnClicked.AddDynamic(this, &UDemoDialogueWidget::OnSendClicked);
	if (TradeButton) TradeButton->OnClicked.AddDynamic(this, &UDemoDialogueWidget::OnTradeClicked);
	if (QuestButton) QuestButton->OnClicked.AddDynamic(this, &UDemoDialogueWidget::OnQuestClicked);
	if (CloseButton) CloseButton->OnClicked.AddDynamic(this, &UDemoDialogueWidget::OnCloseClicked);
	if (PlayerInput) PlayerInput->OnTextCommitted.AddDynamic(this, &UDemoDialogueWidget::OnInputCommitted);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UDemoDialogueWidget::OpenForNPC(ANPCActor* NPC)
{
	CurrentNPC = NPC;
	SetVisibility(ESlateVisibility::Visible);

	if (NPCNameText) NPCNameText->SetText(FText::FromString(NPC->NPCName));
	if (NPCRoleText) NPCRoleText->SetText(FText::FromString(NPC->Role));

	if (TradeButton) TradeButton->SetVisibility(NPC->Role == TEXT("merchant") ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (QuestButton) QuestButton->SetVisibility(NPC->Role == TEXT("quest_giver") ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	DialogueHistory = TEXT("");
	if (DialogueText) DialogueText->SetText(FText::GetEmpty());
	if (EmotionText) EmotionText->SetText(FText::FromString(TEXT("Neutral")));
	if (MoodBar) MoodBar->SetPercent(0.5f);

	AppendSystemText(FString::Printf(TEXT("* You approach %s *"), *NPC->NPCName));

	// Send approach event
	ADemoGameMode* GM = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->NPCManagerInstance)
	{
		GM->NPCManagerInstance->ApproachNPC(NPC);
	}

	if (PlayerInput) PlayerInput->SetKeyboardFocus();
}

void UDemoDialogueWidget::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	CurrentNPC = nullptr;
}

void UDemoDialogueWidget::HandleResponse(ANPCActor* NPC, const FEventResult& Result)
{
	if (NPC != CurrentNPC) return;

	FString Emotion = Result.Response.Emotion.IsEmpty() ? TEXT("neutral") : Result.Response.Emotion;
	if (EmotionText) EmotionText->SetText(FText::FromString(Emotion));
	if (MoodBar) MoodBar->SetPercent(0.5f);

	if (!Result.Response.Action.Type.IsEmpty())
		AppendSystemText(FString::Printf(TEXT("* %s %s *"), *NPC->NPCName, *Result.Response.Action.Type));

	if (!Result.Response.TradeOffer.Item.IsEmpty())
		AppendSystemText(FString::Printf(TEXT("* Offers: %s for %d gold *"), *Result.Response.TradeOffer.Item, Result.Response.TradeOffer.Price));

	FString Dialogue = Result.Response.Dialogue.IsEmpty() ? TEXT("...") : Result.Response.Dialogue;
	AppendDialogue(FString::Printf(TEXT("%s: %s"), *NPC->NPCName, *Dialogue));
}

void UDemoDialogueWidget::OnSendClicked()
{
	if (!CurrentNPC || !PlayerInput) return;

	FString Message = PlayerInput->GetText().ToString().TrimStartAndEnd();
	if (Message.IsEmpty()) return;

	PlayerInput->SetText(FText::GetEmpty());
	AppendDialogue(FString::Printf(TEXT("You: %s"), *Message));

	ADemoGameMode* GM = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->NPCManagerInstance)
	{
		GM->NPCManagerInstance->SayToNPC(CurrentNPC, Message);
	}
}

void UDemoDialogueWidget::OnTradeClicked()
{
	if (!CurrentNPC) return;
	AppendSystemText(TEXT("* You request to trade *"));

	ADemoGameMode* GM = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->NPCManagerInstance)
	{
		GM->NPCManagerInstance->RequestTrade(CurrentNPC, TEXT("I'd like to see your wares"));
	}
}

void UDemoDialogueWidget::OnQuestClicked()
{
	if (!CurrentNPC) return;
	AppendSystemText(TEXT("* You accept the quest *"));

	ADemoGameMode* GM = Cast<ADemoGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM && GM->NPCManagerInstance)
	{
		GM->NPCManagerInstance->AcceptQuest(CurrentNPC);
	}
}

void UDemoDialogueWidget::OnCloseClicked()
{
	ADemoPlayerCharacter* Player = Cast<ADemoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player) Player->OnCloseDialogue();
}

void UDemoDialogueWidget::OnInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
		OnSendClicked();
}

void UDemoDialogueWidget::AppendDialogue(const FString& Text)
{
	if (!DialogueHistory.IsEmpty()) DialogueHistory += TEXT("\n");
	DialogueHistory += Text;
	if (DialogueText) DialogueText->SetText(FText::FromString(DialogueHistory));
}

void UDemoDialogueWidget::AppendSystemText(const FString& Text)
{
	if (!DialogueHistory.IsEmpty()) DialogueHistory += TEXT("\n");
	DialogueHistory += Text;
	if (DialogueText) DialogueText->SetText(FText::FromString(DialogueHistory));
}
