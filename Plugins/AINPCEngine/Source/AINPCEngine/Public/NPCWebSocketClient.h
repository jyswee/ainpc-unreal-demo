#pragma once

#include "CoreMinimal.h"
#include "NPCEngineTypes.h"
#include "IWebSocket.h"
#include "NPCWebSocketClient.generated.h"

class FJsonObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSResponse, FEventResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSConnected, FString, GameId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSSubscribed, const TArray<FString>&, NpcIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSUnsubscribed, const TArray<FString>&, NpcIds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSStateUpdate, FNPCStateUpdate, StateUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSDisconnected, FString, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSError, FString, Error);

UCLASS(BlueprintType, Blueprintable)
class AINPCENGINE_API UNPCWebSocketClient : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AINPCEngine|WebSocket")
    void Connect(const FString& BaseUrl, const FString& ApiKey, const FString& GameId);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine|WebSocket")
    void Disconnect();

    UFUNCTION(BlueprintPure, Category = "AINPCEngine|WebSocket")
    bool IsConnected() const;

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine|WebSocket")
    void SendEvent(const FString& NpcId, const FGameEventRequest& EventData);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine|WebSocket")
    void Subscribe(const TArray<FString>& NpcIds);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine|WebSocket")
    void Unsubscribe(const TArray<FString>& NpcIds);

    /** NPC response to an event sent over the socket */
    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSResponse OnResponse;

    /** Server confirmed the session ("connected" message) — safe to subscribe/send */
    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSConnected OnConnected;

    /** Server acknowledged a subscribe — NpcIds is the full current subscription list */
    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSSubscribed OnSubscribed;

    /** Server acknowledged an unsubscribe — NpcIds is the remaining subscription list */
    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSUnsubscribed OnUnsubscribed;

    /** Push update when a subscribed NPC's mood or location changes */
    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSStateUpdate OnStateUpdate;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSDisconnected OnDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSError OnError;

private:
    TSharedPtr<IWebSocket> Socket;
    bool bIsConnected = false;

    void HandleMessage(const FString& Message);
    static TArray<FString> ParseNpcIds(const TSharedPtr<FJsonObject>& JsonObj);
};
