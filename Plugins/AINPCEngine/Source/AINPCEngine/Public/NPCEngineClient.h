#pragma once

#include "CoreMinimal.h"
#include "NPCEngineTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "NPCEngineClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCReceived, FAINPC, NPC);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCList, const TArray<FAINPC>&, NPCs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventResult, FEventResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchResult, FBatchEventResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchGenerated, FGenerateBatchResponse, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsReceived, FStatsResponse, Stats);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthReceived, FHealthResponse, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNPCDeleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCError, FString, Error);

UCLASS(BlueprintType, Blueprintable)
class AINPCENGINE_API UNPCEngineClient : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void Init(const FString& InBaseUrl, const FString& InApiKey, const FString& InGameId);

    // ── NPC CRUD ────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void CreateNPC(const FCreateNPCRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void GetNPC(const FString& NpcId);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void ListNPCs();

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void UpdateNPC(const FString& NpcId, const FString& JsonUpdates);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void DeleteNPC(const FString& NpcId);

    // ── Events ──────────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void SendEvent(const FString& NpcId, const FGameEventRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void SendBatchEvents(const TArray<FBatchEventItem>& Events);

    // ── Convenience Event Wrappers ──────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void Say(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void Approach(const FString& NpcId, const FString& PlayerId, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void RequestTrade(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void StartCombat(const FString& NpcId, const FString& PlayerId, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void CombatEnded(const FString& NpcId, const FString& PlayerId, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void QuestAccepted(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void QuestCompleted(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void QuestFailed(const FString& NpcId, const FString& PlayerId, const FString& Message, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void PlayerLeft(const FString& NpcId, const FString& PlayerId, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void AmbientTrigger(const FString& NpcId, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void WorldEvent(const FString& NpcId, const FString& Message, const FGameContext& Context);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void NPCInteraction(const FString& NpcId, const FString& OtherNpcId, const FString& Message, const FGameContext& Context);

    // ── Generation ──────────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void GenerateNPC(const FGenerateNPCRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void GenerateBatch(const FGenerateBatchRequest& Request);

    // ── Stats / Health ──────────────────────────────────────────────

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void GetStats();

    UFUNCTION(BlueprintCallable, Category = "AINPCEngine")
    void Health();

    // ── Delegates ───────────────────────────────────────────────────

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnNPCReceived OnNPCReceived;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnNPCList OnNPCList;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnEventResult OnEventResult;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnBatchResult OnBatchResult;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnBatchGenerated OnBatchGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnStatsReceived OnStatsReceived;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnHealthReceived OnHealthReceived;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnNPCDeleted OnNPCDeleted;

    UPROPERTY(BlueprintAssignable, Category = "AINPCEngine")
    FOnNPCError OnError;

private:
    FString BaseUrl;
    FString ApiKey;
    FString GameId;

    TSharedRef<IHttpRequest> CreateRequest(const FString& Verb, const FString& Path) const;
    void SendEventInternal(const FString& NpcId, const FString& EventType, const FString& PlayerId, const FString& Message, const FGameContext& Context);
};
