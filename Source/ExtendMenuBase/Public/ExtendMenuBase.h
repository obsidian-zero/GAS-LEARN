#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FExtendMenuBaseModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual ~FExtendMenuBaseModule() {}

private:

    // Extend menu here
    void ExtendMenuByFExtendMenuBaseModule();

    void MakeExistingMainMenuEntry(FMenuBuilder& MenuBuilder);

    void ExistingMainMenuEntryAction();

    void MakeNewMenuBarEntry(FMenuBarBuilder& MenuBarBuilder);
    
    void MakeNewMainMenuEntry(FMenuBuilder& MenuBarBuilder);

    void NewMainMenuEntryAction();
};
