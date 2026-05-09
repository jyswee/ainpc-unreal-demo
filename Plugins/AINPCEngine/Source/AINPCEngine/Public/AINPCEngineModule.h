#pragma once

#include "Modules/ModuleManager.h"

class FAINPCEngineModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
