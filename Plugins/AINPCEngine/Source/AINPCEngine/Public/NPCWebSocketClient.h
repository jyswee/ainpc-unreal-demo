#pragma once

#include "CoreMinimal.h"
#include "NPCEngineTypes.h"
#include "IWebSocket.h"
#include "NPCWebSocketClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWSResponse, FEventResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWSConnected);
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

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSResponse OnResponse;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSConnected OnConnected;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSDisconnected OnDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine|WebSocket")
    FOnWSError OnError;

private:
    TSharedPtr<IWebSocket> Socket;
    bool bIsConnected = false;

    void HandleMessage(const FString& Message);
};
