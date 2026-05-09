# AINPCEngine Unreal Demo

A polished demo showing AI-powered NPCs in Unreal Engine 5.3+ using the [AINPCEngine](https://ainpcengine.com) SDK plugin. Walk around a medieval village and have natural conversations with 3 intelligent NPCs who remember you, trade goods, and offer quests.

## Features

- **Procedural 3D village** — environment spawns at runtime from engine primitives
- **3 AI NPCs** with distinct personalities:
  - **Elara** (Merchant, gold) — shrewd trader, haggle for potions and weapons
  - **Theron** (Guard, blue) — stoic veteran, suspicious of strangers
  - **Old Miriam** (Quest Giver, green) — cryptic seer, speaks in riddles
- **Natural language dialogue** — say anything, NPCs respond intelligently
- **Persistent memory** — NPCs remember your conversations
- **Dynamic emotions** — glowing emotion orbs above NPCs change color in real-time
- **Trade & quest systems** — context-aware buttons for each NPC type
- **Full C++ source** — all BlueprintCallable, extendable via Blueprints

## Prerequisites

- [Unreal Engine 5.3+](https://www.unrealengine.com/download)
- Visual Studio 2022 or Rider
- [Docker](https://docker.com) & Docker Compose
- An AINPCEngine API key (get one at [ainpcengine.com](https://ainpcengine.com))

## Quick Start

### 1. Start AINPCEngine

```bash
git clone https://github.com/jyswee/AINPCEngine.git
cd AINPCEngine
docker compose up -d
```

### 2. Clone this demo

```bash
git clone https://github.com/jyswee/ainpc-unreal-demo.git
```

### 3. Configure

Edit `Source/AINPCDemo/DemoConfig.h` and set your API key:

```cpp
#define AINPC_API_URL    TEXT("http://localhost:16745")
#define AINPC_API_KEY    TEXT("dev-api-key-change-me")
#define AINPC_GAME_ID    TEXT("ainpc-unreal-demo")
```

> **Note**: The default `docker-compose.yml` exposes port **16745**. If you deployed via Portainer, the port is typically **18542**.

### 4. Build & Play

1. Right-click `AINPCDemo.uproject` → **Generate Visual Studio Project Files**
2. Open in Visual Studio / Rider and build
3. Open in UE Editor
4. Create a new **Empty Level**, save as `Content/Maps/DemoVillage`
5. Set **World Settings → GameMode Override** to `DemoGameMode`
6. Press **Play** (PIE)
7. Walk with **WASD**, look with **mouse**, press **E** near NPCs

## Project Structure

```
ainpc-unreal-demo/
├── AINPCDemo.uproject
├── Config/
│   ├── DefaultEngine.ini          # GameMode & map settings
│   ├── DefaultGame.ini            # Project metadata
│   └── DefaultInput.ini           # WASD + E input bindings
├── Plugins/
│   └── AINPCEngine/               # AINPCEngine SDK plugin (REST + WebSocket)
└── Source/AINPCDemo/
    ├── DemoConfig.h               # API configuration (edit this)
    ├── DemoGameMode.cpp/h         # Spawns village + NPCs on BeginPlay
    ├── DemoPlayerCharacter.cpp/h  # WASD movement, mouse look, interact
    ├── NPCActor.cpp/h             # NPC with mesh, name label, emotion orb
    ├── NPCManager.cpp/h           # API client wrapper, NPC registry
    ├── VillageBuilder.cpp/h       # Procedural environment (ground, walls, props)
    └── DemoDialogueWidget.cpp/h   # UMG dialogue panel
```

## How It Works

1. **On BeginPlay**, `DemoGameMode` builds the village via `VillageBuilder` and initializes `NPCManager`
2. **`NPCManager`** connects to the AINPCEngine API and registers all 3 NPCs with their personalities
3. **Walking near** an NPC triggers overlap → sets `NearestNPC` on the player character
4. **Press E** → sends `player_approached` event, opens dialogue widget
5. **Type anything** → `NPCManager::SayToNPC()` sends dialogue, receives AI response
6. **Emotion orbs** update color via dynamic material instances
7. **Trade/Quest** buttons send specialized events for merchant/quest giver NPCs

## Extending with Blueprints

All core classes are `BlueprintCallable`. You can:
- Create Blueprint subclasses of `DemoGameMode`, `DemoPlayerCharacter`, `NPCActor`
- Design a UMG Widget Blueprint for the dialogue panel (bind to `NPCNameText`, `DialogueText`, etc.)
- Add Blueprint logic for inventory, quest tracking, or combat integration

## License

MIT License — [Tyga.Cloud Ltd](https://tyga.cloud)
