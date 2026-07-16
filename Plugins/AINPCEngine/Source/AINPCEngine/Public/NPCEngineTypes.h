#pragma once

#include "CoreMinimal.h"
#include "NPCEngineTypes.generated.h"

// ── NPC Types ──────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAINPCPersonality
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Traits;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString SpeechStyle;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Backstory;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Values;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Quirks;
};

USTRUCT(BlueprintType)
struct FAINPCMood
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Emotion;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Intensity = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Cause;
};

USTRUCT(BlueprintType)
struct FNPCInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Item;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Price = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Quantity = 0;
};

// ── Cognitive Types ────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FGeneratedPersona
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Age = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Gender;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Ethnicity;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Occupation;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Income;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float WorkEthic = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    bool Literacy = false;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Expertise;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString MaritalStatus;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Children = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> FamilyNpcIds;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Openness = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Conscientiousness = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Extraversion = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Agreeableness = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Neuroticism = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Height;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Build;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> DistinguishingFeatures;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Vices;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Virtues;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Fears;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Ambitions;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float RoutinePreference = 0.f;
};

USTRUCT(BlueprintType)
struct FScheduleSlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Activity;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Location;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 StartHour = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 EndHour = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<int32> Days;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 JitterMinutes = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Priority = 0;
};

USTRUCT(BlueprintType)
struct FScheduleOverride
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Reason;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FScheduleSlot Slot;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString ExpiresAt;
};

USTRUCT(BlueprintType)
struct FNPCSchedule
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NpcId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FScheduleSlot> Slots;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FScheduleOverride> Overrides;
};

USTRUCT(BlueprintType)
struct FTraumaEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString TraumaEvent;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Severity = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString OccurredAt;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> Triggers;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float HealingRate = 0.f;
};

USTRUCT(BlueprintType)
struct FNPCGoal
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Id;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Description;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Priority = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Progress = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Deadline;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> BlockedBy;
};

USTRUCT(BlueprintType)
struct FEmotionLayer
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Emotion;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Intensity = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Cause;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float DecayRate = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString StartedAt;
};

USTRUCT(BlueprintType)
struct FNPCPsychology
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Stress = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FTraumaEntry> Trauma;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FNPCGoal> Goals;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FEmotionLayer> Emotions;
};

// ── NPC ────────────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FAINPC
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Id;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FAINPCPersonality Personality;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Role;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Faction;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Location;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FNPCInventoryItem> Inventory;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FAINPCMood Mood;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString CreatedAt;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString GameId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FGeneratedPersona Persona;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FNPCSchedule Schedule;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FNPCPsychology Psychology;
};

USTRUCT(BlueprintType)
struct FCreateNPCRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FAINPCPersonality Personality;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Role;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Faction;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Location;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FNPCInventoryItem> Inventory;
};

// ── Event Types ────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FGameContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Location;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString TimeOfDay;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Weather;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> NearbyNPCs;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 PlayerReputation = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 PlayerLevel = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> ActiveQuests;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> RecentEvents;
};

USTRUCT(BlueprintType)
struct FGameEventRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Type;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString PlayerId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Message;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FGameContext Context;
};

// ── Response Types ─────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FNPCAction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Type;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Target;
};

USTRUCT(BlueprintType)
struct FTradeOffer
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Item;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Price = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Condition;
};

USTRUCT(BlueprintType)
struct FQuestUpdate
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString QuestId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Status;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NextObjective;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Reward;
};

USTRUCT(BlueprintType)
struct FCombatDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Action;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Target;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Reasoning;
};

USTRUCT(BlueprintType)
struct FNPCResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString EventId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NpcId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Dialogue;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Emotion;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FNPCAction Action;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FTradeOffer TradeOffer;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FQuestUpdate QuestUpdate;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FCombatDecision CombatDecision;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    bool MemoryUpdated = false;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Timestamp;
};

USTRUCT(BlueprintType)
struct FTokenUsage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Input = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Output = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Calls = 0;
};

USTRUCT(BlueprintType)
struct FEventResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FNPCResponse Response;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FTokenUsage TokenUsage;
};

// ── Generate / Batch Types ─────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FGenerateNPCRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Role;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Name;
};

USTRUCT(BlueprintType)
struct FGenerateBatchRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 Count = 5;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Role;
};

USTRUCT(BlueprintType)
struct FSocialLink
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString FromId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString ToId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Type;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Strength = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Since;
};

USTRUCT(BlueprintType)
struct FGenerateBatchResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FAINPC> Npcs;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FSocialLink> SocialLinks;
};

USTRUCT(BlueprintType)
struct FBatchEventItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Type;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NpcId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString PlayerId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Message;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FGameContext Context;
};

USTRUCT(BlueprintType)
struct FBatchEventResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FEventResult> Results;
};

// ── Stats Types ────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FStatsExpert
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Id;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Domain;
};

USTRUCT(BlueprintType)
struct FStatsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FStatsExpert> Experts;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 TotalExperts = 0;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    TArray<FString> CognitiveFeatures;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Uptime = 0.f;
};

// ── Voice (TTS) ────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FSpeakResponse
{
    GENERATED_BODY()

    /** Base64-encoded audio payload */
    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Audio;

    /** Audio format (e.g. "wav") */
    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Format;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Voice;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Provider;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NpcId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NpcName;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 TextLength = 0;
};

// ── Errors ─────────────────────────────────────────────────────────────

/**
 * Structured API error.
 *
 * Codes worth handling in-game:
 *   "payment_required"    (HTTP 402) — account has no active card / trial not
 *                          started. Send the player of your dashboard to
 *                          PaymentUrl to add a card (7-day free trial, $0 today).
 *   "npc_limit"           (HTTP 402) — plan NPC cap reached, upgrade to add more.
 *   "feature_not_in_plan" (HTTP 402) — endpoint not included in current plan.
 *   "interaction_cap"     (HTTP 429) — monthly included interactions used up.
 */
USTRUCT(BlueprintType)
struct FAINPCError
{
    GENERATED_BODY()

    /** HTTP status code (0 = network/transport failure) */
    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    int32 HttpStatus = 0;

    /** Machine-readable error code, e.g. "payment_required", "npc_limit", "interaction_cap" */
    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Code;

    /** Human-readable error message */
    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Message;

    /** Where to resolve billing issues (set on 402 payment_required and plan-limit errors) */
    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString PaymentUrl;
};

// ── WebSocket ──────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FNPCStateUpdate
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString NpcId;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FAINPCMood Mood;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Location;
};

// ── Health ─────────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct FHealthResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Status;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    FString Service;

    UPROPERTY(BlueprintReadWrite, Category = "AINPCEngine")
    float Uptime = 0.f;
};
