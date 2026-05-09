#include "NPCEngineClient.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"

void UNPCEngineClient::Init(const FString& InBaseUrl, const FString& InApiKey, const FString& InGameId)
{
    BaseUrl = InBaseUrl.TrimEnd();
    // Remove trailing slash
    while (BaseUrl.EndsWith(TEXT("/")))
    {
        BaseUrl.LeftChopInline(1);
    }
    ApiKey = InApiKey;
    GameId = InGameId;
}

TSharedRef<IHttpRequest> UNPCEngineClient::CreateRequest(const FString& Verb, const FString& Path) const
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(BaseUrl + Path);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("x-api-key"), ApiKey);
    Request->SetHeader(TEXT("x-game-id"), GameId);
    return Request;
}

// ── NPC CRUD ────────────────────────────────────────────────────────────

void UNPCEngineClient::CreateNPC(const FCreateNPCRequest& Request)
{
    auto HttpRequest = CreateRequest(TEXT("POST"), TEXT("/api/npcs"));
    FString Body;
    FJsonObjectConverter::UStructToJsonObjectString(Request, Body);
    HttpRequest->SetContentAsString(Body);
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("CreateNPC request failed")); return; }
        FAINPC NPC;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &NPC);
        OnNPCReceived.Broadcast(NPC);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::GetNPC(const FString& NpcId)
{
    auto HttpRequest = CreateRequest(TEXT("GET"), FString::Printf(TEXT("/api/npcs/%s"), *NpcId));
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("GetNPC request failed")); return; }
        FAINPC NPC;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &NPC);
        OnNPCReceived.Broadcast(NPC);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::ListNPCs()
{
    auto HttpRequest = CreateRequest(TEXT("GET"), TEXT("/api/npcs"));
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("ListNPCs request failed")); return; }

        TArray<FAINPC> NPCs;
        TSharedPtr<FJsonValue> JsonValue;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Res->GetContentAsString());
        if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue->Type == EJson::Array)
        {
            for (const auto& Item : JsonValue->AsArray())
            {
                FAINPC NPC;
                FJsonObjectConverter::JsonObjectToUStruct(Item->AsObject().ToSharedRef(), &NPC);
                NPCs.Add(NPC);
            }
        }
        OnNPCList.Broadcast(NPCs);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::UpdateNPC(const FString& NpcId, const FString& JsonUpdates)
{
    auto HttpRequest = CreateRequest(TEXT("PATCH"), FString::Printf(TEXT("/api/npcs/%s"), *NpcId));
    HttpRequest->SetContentAsString(JsonUpdates);
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("UpdateNPC request failed")); return; }
        FAINPC NPC;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &NPC);
        OnNPCReceived.Broadcast(NPC);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::DeleteNPC(const FString& NpcId)
{
    auto HttpRequest = CreateRequest(TEXT("DELETE"), FString::Printf(TEXT("/api/npcs/%s"), *NpcId));
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("DeleteNPC request failed")); return; }
        OnNPCDeleted.Broadcast();
    });
    HttpRequest->ProcessRequest();
}

// ── Events ──────────────────────────────────────────────────────────────

void UNPCEngineClient::SendEvent(const FString& NpcId, const FGameEventRequest& Request)
{
    auto HttpRequest = CreateRequest(TEXT("POST"), FString::Printf(TEXT("/api/npcs/%s/event"), *NpcId));
    FString Body;
    FJsonObjectConverter::UStructToJsonObjectString(Request, Body);
    HttpRequest->SetContentAsString(Body);
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("SendEvent request failed")); return; }
        FEventResult Result;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Result);
        OnEventResult.Broadcast(Result);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::SendBatchEvents(const TArray<FBatchEventItem>& Events)
{
    auto HttpRequest = CreateRequest(TEXT("POST"), TEXT("/api/events"));

    // Build {"events": [...]} wrapper
    TSharedPtr<FJsonObject> Wrapper = MakeShareable(new FJsonObject());
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    for (const auto& Evt : Events)
    {
        TSharedPtr<FJsonObject> EvtObj = FJsonObjectConverter::UStructToJsonObject(Evt);
        JsonArray.Add(MakeShareable(new FJsonValueObject(EvtObj)));
    }
    Wrapper->SetArrayField(TEXT("events"), JsonArray);

    FString Body;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Body);
    FJsonSerializer::Serialize(Wrapper.ToSharedRef(), Writer);
    HttpRequest->SetContentAsString(Body);

    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("SendBatchEvents request failed")); return; }
        FBatchEventResult Result;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Result);
        OnBatchResult.Broadcast(Result);
    });
    HttpRequest->ProcessRequest();
}

// ── Convenience Event Wrappers ──────────────────────────────────────────

void UNPCEngineClient::SendEventInternal(const FString& NpcId, const FString& EventType, const FString& PlayerId, const FString& Message, const FGameContext& Context)
{
    FGameEventRequest Request;
    Request.Type = EventType;
    Request.PlayerId = PlayerId;
    Request.Message = Message;
    Request.Context = Context;
    SendEvent(NpcId, Request);
}

void UNPCEngineClient::Say(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("player_dialogue"), PlayerId, Message, Context);
}

void UNPCEngineClient::Approach(const FString& NpcId, const FString& PlayerId, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("player_approached"), PlayerId, TEXT(""), Context);
}

void UNPCEngineClient::RequestTrade(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("trade_requested"), PlayerId, Message, Context);
}

void UNPCEngineClient::StartCombat(const FString& NpcId, const FString& PlayerId, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("combat_started"), PlayerId, TEXT(""), Context);
}

void UNPCEngineClient::CombatEnded(const FString& NpcId, const FString& PlayerId, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("combat_ended"), PlayerId, TEXT(""), Context);
}

void UNPCEngineClient::QuestAccepted(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("quest_accepted"), PlayerId, Message, Context);
}

void UNPCEngineClient::QuestCompleted(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("quest_completed"), PlayerId, Message, Context);
}

void UNPCEngineClient::QuestFailed(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("quest_failed"), PlayerId, Message, Context);
}

void UNPCEngineClient::PlayerLeft(const FString& NpcId, const FString& PlayerId, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("player_left"), PlayerId, TEXT(""), Context);
}

void UNPCEngineClient::AmbientTrigger(const FString& NpcId, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("ambient_trigger"), TEXT(""), TEXT(""), Context);
}

void UNPCEngineClient::WorldEvent(const FString& NpcId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("world_event"), TEXT(""), Message, Context);
}

void UNPCEngineClient::NPCInteraction(const FString& NpcId, const FString& OtherNpcId, const FString& Message, const FGameContext& Context)
{
    SendEventInternal(NpcId, TEXT("npc_interaction"), OtherNpcId, Message, Context);
}

// ── Generation ──────────────────────────────────────────────────────────

void UNPCEngineClient::GenerateNPC(const FGenerateNPCRequest& Request)
{
    auto HttpRequest = CreateRequest(TEXT("POST"), TEXT("/api/npcs/generate"));
    FString Body;
    FJsonObjectConverter::UStructToJsonObjectString(Request, Body);
    HttpRequest->SetContentAsString(Body);
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("GenerateNPC request failed")); return; }
        FAINPC NPC;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &NPC);
        OnNPCReceived.Broadcast(NPC);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::GenerateBatch(const FGenerateBatchRequest& Request)
{
    auto HttpRequest = CreateRequest(TEXT("POST"), TEXT("/api/npcs/generate-batch"));
    FString Body;
    FJsonObjectConverter::UStructToJsonObjectString(Request, Body);
    HttpRequest->SetContentAsString(Body);
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("GenerateBatch request failed")); return; }
        FGenerateBatchResponse Result;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Result);
        OnBatchGenerated.Broadcast(Result);
    });
    HttpRequest->ProcessRequest();
}

// ── Stats / Health ──────────────────────────────────────────────────────

void UNPCEngineClient::GetStats()
{
    auto HttpRequest = CreateRequest(TEXT("GET"), TEXT("/api/stats"));
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("GetStats request failed")); return; }
        FStatsResponse Stats;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &Stats);
        OnStatsReceived.Broadcast(Stats);
    });
    HttpRequest->ProcessRequest();
}

void UNPCEngineClient::Health()
{
    auto HttpRequest = CreateRequest(TEXT("GET"), TEXT("/api/health"));
    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
    {
        if (!bSuccess || !Res.IsValid()) { OnError.Broadcast(TEXT("Health request failed")); return; }
        FHealthResponse HealthResp;
        FJsonObjectConverter::JsonObjectStringToUStruct(Res->GetContentAsString(), &HealthResp);
        OnHealthReceived.Broadcast(HealthResp);
    });
    HttpRequest->ProcessRequest();
}
