#include "AINPCEngineModule.h"
#include "WebSocketsModule.h"
#include "Modules/ModuleManager.h"

void FAINPCEngineModule::StartupModule()
{
    FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");
}

void FAINPCEngineModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FAINPCEngineModule, AINPCEngine)
