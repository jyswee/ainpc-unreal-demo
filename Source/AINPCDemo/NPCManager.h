#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NPCEngineClient.h"
#include "NPCEngineTypes.h"
#include "NPCManager.generated.h"

class ANPCActor;

UCLASS()
class AINPCDEMO_API UNPCManager : public UObject
{
	GENERATED_BODY()

public:
	void Initialize();

	void RegisterNPC(ANPCActor* NPC);
	void ApproachNPC(ANPCActor* NPC);
	void SayToNPC(ANPCActor* NPC, const FString& Message);
	void RequestTrade(ANPCActor* NPC, const FString& Message);
	void AcceptQuest(ANPCActor* NPC);
	void LeaveNPC(ANPCActor* NPC);

	UPROPERTY()
	UNPCEngineClient* Client;

	UPROPERTY()
	TMap<FString, ANPCActor*> NPCRegistry;

	bool bIsConnected = false;

	// Delegate for UI to bind
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNPCResponse, ANPCActor*, NPC, FEventResult, Result);
	UPROPERTY(BlueprintAssignable)
	FOnNPCResponse OnNPCResponse;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionStatus, bool, Connected);
	UPROPERTY(BlueprintAssignable)
	FOnConnectionStatus OnConnectionStatus;

private:
	FGameContext BuildContext(ANPCActor* NPC);

	UFUNCTION()
	void OnHealthReceived(FHealthResponse Response);

	UFUNCTION()
	void OnNPCReceived(FAINPC NPC);

	UFUNCTION()
	void OnEventResult(FEventResult Result);

	UFUNCTION()
	void OnError(FString Error);

	// Track which NPC the current event is for
	ANPCActor* PendingNPC = nullptr;
	ANPCActor* PendingRegisterNPC = nullptr;
};
