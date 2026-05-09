#include "NPCManager.h"
#include "NPCActor.h"
#include "DemoConfig.h"

void UNPCManager::Initialize()
{
	Client = NewObject<UNPCEngineClient>(this);
	Client->Init(AINPC_API_URL, AINPC_API_KEY, AINPC_GAME_ID);

	Client->OnHealthReceived.AddDynamic(this, &UNPCManager::OnHealthReceived);
	Client->OnNPCReceived.AddDynamic(this, &UNPCManager::OnNPCReceived);
	Client->OnEventResult.AddDynamic(this, &UNPCManager::OnEventResult);
	Client->OnError.AddDynamic(this, &UNPCManager::OnError);

	Client->Health();
}

void UNPCManager::RegisterNPC(ANPCActor* NPC)
{
	if (!bIsConnected || !NPC) return;

	FCreateNPCRequest Request;
	Request.Name = NPC->NPCName;
	Request.Role = NPC->Role;
	Request.Location = NPC->NPCLocation;

	FAINPCPersonality Personality;
	if (NPC->Role == TEXT("merchant"))
	{
		Personality.Traits = { TEXT("shrewd"), TEXT("friendly"), TEXT("cunning"), TEXT("silver-tongued") };
		Personality.SpeechStyle = TEXT("Enthusiastic medieval trader, uses coin metaphors, always upselling");
		Personality.Backstory = TEXT("Once a travelling merchant who settled in the village after her caravan was attacked.");
		Personality.Values = { TEXT("profit"), TEXT("fairness"), TEXT("community") };
		Personality.Quirks = { TEXT("rubs coins together when nervous"), TEXT("winks when offering a deal") };
		Request.Faction = TEXT("Merchants Guild");
	}
	else if (NPC->Role == TEXT("guard"))
	{
		Personality.Traits = { TEXT("stoic"), TEXT("dutiful"), TEXT("suspicious"), TEXT("protective") };
		Personality.SpeechStyle = TEXT("Clipped military speech, formal address, rarely jokes");
		Personality.Backstory = TEXT("A veteran soldier who retired to guard duty after a knee injury.");
		Personality.Values = { TEXT("order"), TEXT("duty"), TEXT("honor") };
		Personality.Quirks = { TEXT("constantly scans the perimeter"), TEXT("hand always near sword hilt") };
		Request.Faction = TEXT("Village Watch");
	}
	else if (NPC->Role == TEXT("quest_giver"))
	{
		Personality.Traits = { TEXT("mysterious"), TEXT("wise"), TEXT("cryptic"), TEXT("patient") };
		Personality.SpeechStyle = TEXT("Speaks in riddles and half-truths, references prophecies");
		Personality.Backstory = TEXT("An elderly seer who has lived in the village longer than anyone can remember.");
		Personality.Values = { TEXT("knowledge"), TEXT("balance"), TEXT("destiny") };
		Personality.Quirks = { TEXT("eyes glow faintly when giving quests"), TEXT("hums an unknown melody") };
		Request.Faction = TEXT("Seers Circle");
	}

	Request.Personality = Personality;
	PendingRegisterNPC = NPC;
	Client->CreateNPC(Request);
}

void UNPCManager::ApproachNPC(ANPCActor* NPC)
{
	if (!bIsConnected || !NPC || NPC->ServerNPCId.IsEmpty()) return;

	FGameContext Context = BuildContext(NPC);
	FGameEventRequest Event;
	Event.Type = TEXT("player_approached");
	Event.PlayerId = TEXT("player_1");
	Event.Context = Context;

	PendingNPC = NPC;
	Client->SendEvent(NPC->ServerNPCId, Event);
}

void UNPCManager::SayToNPC(ANPCActor* NPC, const FString& Message)
{
	if (!bIsConnected || !NPC || NPC->ServerNPCId.IsEmpty()) return;

	FGameContext Context = BuildContext(NPC);
	PendingNPC = NPC;
	Client->Say(NPC->ServerNPCId, TEXT("player_1"), Message, Context);
}

void UNPCManager::RequestTrade(ANPCActor* NPC, const FString& Message)
{
	if (!bIsConnected || !NPC || NPC->ServerNPCId.IsEmpty()) return;

	FGameContext Context = BuildContext(NPC);
	PendingNPC = NPC;
	Client->RequestTrade(NPC->ServerNPCId, TEXT("player_1"), Message, Context);
}

void UNPCManager::AcceptQuest(ANPCActor* NPC)
{
	if (!bIsConnected || !NPC || NPC->ServerNPCId.IsEmpty()) return;

	FGameContext Context = BuildContext(NPC);
	PendingNPC = NPC;
	Client->QuestAccepted(NPC->ServerNPCId, TEXT("player_1"), TEXT("I accept your quest"), Context);
}

void UNPCManager::LeaveNPC(ANPCActor* NPC)
{
	if (!bIsConnected || !NPC || NPC->ServerNPCId.IsEmpty()) return;

	FGameContext Context = BuildContext(NPC);
	PendingNPC = NPC;
	Client->PlayerLeft(NPC->ServerNPCId, TEXT("player_1"), Context);
}

FGameContext UNPCManager::BuildContext(ANPCActor* NPC)
{
	FGameContext Context;
	Context.Location = NPC->NPCLocation;
	Context.Weather = TEXT("sunny");
	Context.PlayerReputation = 50;
	Context.PlayerLevel = 1;

	int32 Hour = FDateTime::Now().GetHour();
	if (Hour >= 6 && Hour < 12) Context.TimeOfDay = TEXT("morning");
	else if (Hour >= 12 && Hour < 17) Context.TimeOfDay = TEXT("afternoon");
	else if (Hour >= 17 && Hour < 21) Context.TimeOfDay = TEXT("evening");
	else Context.TimeOfDay = TEXT("night");

	for (auto& Pair : NPCRegistry)
	{
		if (Pair.Value != NPC)
			Context.NearbyNPCs.Add(Pair.Value->NPCName);
	}

	return Context;
}

void UNPCManager::OnHealthReceived(FHealthResponse Response)
{
	bIsConnected = (Response.Status == TEXT("ok") || Response.Status == TEXT("healthy"));
	UE_LOG(LogTemp, Log, TEXT("[AINPCEngine] Connection: %s"), bIsConnected ? TEXT("Connected") : TEXT("Offline"));
	OnConnectionStatus.Broadcast(bIsConnected);
}

void UNPCManager::OnNPCReceived(FAINPC NPC)
{
	if (PendingRegisterNPC)
	{
		PendingRegisterNPC->ServerNPCId = NPC.Id;
		NPCRegistry.Add(NPC.Id, PendingRegisterNPC);
		UE_LOG(LogTemp, Log, TEXT("[AINPCEngine] Registered: %s -> %s"), *PendingRegisterNPC->NPCName, *NPC.Id);
		PendingRegisterNPC = nullptr;
	}
}

void UNPCManager::OnEventResult(FEventResult Result)
{
	if (PendingNPC)
	{
		FString Emotion = Result.Response.Emotion.IsEmpty() ? TEXT("neutral") : Result.Response.Emotion;
		PendingNPC->SetEmotion(Emotion, 0.5f);
		OnNPCResponse.Broadcast(PendingNPC, Result);
	}
}

void UNPCManager::OnError(FString Error)
{
	UE_LOG(LogTemp, Error, TEXT("[AINPCEngine] Error: %s"), *Error);
}
