#include "NPCWebSocketClient.h"
#include "WebSocketsModule.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

void UNPCWebSocketClient::Connect(const FString& BaseUrl, const FString& ApiKey, const FString& GameId)
{
    FString Url = FString::Printf(TEXT("%s?apiKey=%s&gameId=%s"), *BaseUrl, *ApiKey, *GameId);

    Socket = FWebSocketsModule::Get().CreateWebSocket(Url, TEXT("ws"));

    Socket->OnConnected().AddLambda([this]()
    {
        // Socket is open; OnConnected broadcasts once the server sends its
        // "connected" handshake message (see HandleMessage).
        bIsConnected = true;
    });

    Socket->OnConnectionError().AddLambda([this](const FString& Error)
    {
        bIsConnected = false;
        OnError.Broadcast(FString::Printf(TEXT("Connection error: %s"), *Error));
    });

    Socket->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean)
    {
        bIsConnected = false;
        OnDisconnected.Broadcast(Reason);
    });

    Socket->OnMessage().AddLambda([this](const FString& Message)
    {
        HandleMessage(Message);
    });

    Socket->Connect();
}

void UNPCWebSocketClient::Disconnect()
{
    if (Socket.IsValid() && bIsConnected)
    {
        Socket->Close();
        bIsConnected = false;
    }
}

bool UNPCWebSocketClient::IsConnected() const
{
    return bIsConnected;
}

void UNPCWebSocketClient::SendEvent(const FString& NpcId, const FGameEventRequest& EventData)
{
    if (!bIsConnected) return;

    TSharedPtr<FJsonObject> DataObj = FJsonObjectConverter::UStructToJsonObject(EventData);
    DataObj->SetStringField(TEXT("npcId"), NpcId);

    TSharedPtr<FJsonObject> Wrapper = MakeShareable(new FJsonObject());
    Wrapper->SetStringField(TEXT("action"), TEXT("event"));
    Wrapper->SetObjectField(TEXT("data"), DataObj);

    FString Json;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Wrapper.ToSharedRef(), Writer);

    Socket->Send(Json);
}

void UNPCWebSocketClient::Subscribe(const TArray<FString>& NpcIds)
{
    if (!bIsConnected) return;

    TSharedPtr<FJsonObject> Msg = MakeShareable(new FJsonObject());
    Msg->SetStringField(TEXT("action"), TEXT("subscribe"));

    TArray<TSharedPtr<FJsonValue>> Ids;
    for (const auto& Id : NpcIds)
    {
        Ids.Add(MakeShareable(new FJsonValueString(Id)));
    }
    Msg->SetArrayField(TEXT("npcIds"), Ids);

    FString Json;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Msg.ToSharedRef(), Writer);

    Socket->Send(Json);
}

void UNPCWebSocketClient::Unsubscribe(const TArray<FString>& NpcIds)
{
    if (!bIsConnected) return;

    TSharedPtr<FJsonObject> Msg = MakeShareable(new FJsonObject());
    Msg->SetStringField(TEXT("action"), TEXT("unsubscribe"));

    TArray<TSharedPtr<FJsonValue>> Ids;
    for (const auto& Id : NpcIds)
    {
        Ids.Add(MakeShareable(new FJsonValueString(Id)));
    }
    Msg->SetArrayField(TEXT("npcIds"), Ids);

    FString Json;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Json);
    FJsonSerializer::Serialize(Msg.ToSharedRef(), Writer);

    Socket->Send(Json);
}

void UNPCWebSocketClient::HandleMessage(const FString& Message)
{
    TSharedPtr<FJsonObject> JsonObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);

    if (!FJsonSerializer::Deserialize(Reader, JsonObj) || !JsonObj.IsValid())
    {
        OnError.Broadcast(TEXT("Failed to parse WebSocket message"));
        return;
    }

    FString Type = JsonObj->GetStringField(TEXT("type"));

    if (Type == TEXT("response"))
    {
        const TSharedPtr<FJsonObject>* DataObj;
        if (JsonObj->TryGetObjectField(TEXT("data"), DataObj))
        {
            FEventResult Result;
            FJsonObjectConverter::JsonObjectToUStruct((*DataObj).ToSharedRef(), &Result);
            OnResponse.Broadcast(Result);
        }
    }
    else if (Type == TEXT("connected"))
    {
        FString GameId;
        JsonObj->TryGetStringField(TEXT("gameId"), GameId);
        OnConnected.Broadcast(GameId);
    }
    else if (Type == TEXT("subscribed"))
    {
        OnSubscribed.Broadcast(ParseNpcIds(JsonObj));
    }
    else if (Type == TEXT("unsubscribed"))
    {
        OnUnsubscribed.Broadcast(ParseNpcIds(JsonObj));
    }
    else if (Type == TEXT("state_update"))
    {
        FNPCStateUpdate Update;
        JsonObj->TryGetStringField(TEXT("npcId"), Update.NpcId);

        const TSharedPtr<FJsonObject>* DataObj;
        if (JsonObj->TryGetObjectField(TEXT("data"), DataObj))
        {
            const TSharedPtr<FJsonObject>* MoodObj;
            if ((*DataObj)->TryGetObjectField(TEXT("mood"), MoodObj))
            {
                FJsonObjectConverter::JsonObjectToUStruct((*MoodObj).ToSharedRef(), &Update.Mood);
            }
            (*DataObj)->TryGetStringField(TEXT("location"), Update.Location);
        }
        OnStateUpdate.Broadcast(Update);
    }
    else if (Type == TEXT("error"))
    {
        FString ErrorMsg = JsonObj->GetStringField(TEXT("message"));
        OnError.Broadcast(FString::Printf(TEXT("Server error: %s"), *ErrorMsg));
    }
}

TArray<FString> UNPCWebSocketClient::ParseNpcIds(const TSharedPtr<FJsonObject>& JsonObj)
{
    TArray<FString> NpcIds;
    const TArray<TSharedPtr<FJsonValue>>* IdsArray;
    if (JsonObj->TryGetArrayField(TEXT("npcIds"), IdsArray))
    {
        for (const auto& IdValue : *IdsArray)
        {
            FString Id;
            if (IdValue->TryGetString(Id))
            {
                NpcIds.Add(Id);
            }
        }
    }
    return NpcIds;
}
