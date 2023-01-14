#pragma once

#include "gigperformer/sdk/GPMidiMessages.h"
#include "gigperformer/sdk/GPUtils.h"
#include "gigperformer/sdk/GigPerformerAPI.h"
#include "gigperformer/sdk/types.h"
#include <windows.h>
#include <powerbase.h>

// This line is magic!
using namespace std;

/// <summary>
/// Global, because one fits all
/// </summary>
SYSTEM_BATTERY_STATE sbs;
SYSTEM_POWER_STATUS sps;

void RefreshState()
{
    CallNtPowerInformation((POWER_INFORMATION_LEVEL)5, NULL, 0, &sbs, sizeof(sbs));
    CallNtPowerInformation((POWER_INFORMATION_LEVEL)5, NULL, 0, &sbs, sizeof(sbs));
    GetSystemPowerStatus(&sps);
}

int GetACLineStatus()
{
    RefreshState();
    return sps.ACLineStatus;
}

int GetBatteryPercent()
{
    RefreshState();
    return sps.BatteryLifePercent;
}

int GetBatteryCharging()
{
    RefreshState();
    return sbs.Charging;
}

int GetBatteryDischarging()
{
    RefreshState();
    return sbs.Discharging;
}

long GetRate()
{
    RefreshState();
    return sbs.Rate;
}

int GetBatteryPresent()
{
    RefreshState();
    return sbs.BatteryPresent;
}

int GetBatteryLifeTime()
{
    RefreshState();
    return sbs.EstimatedTime;
}


class LibMain : public gigperformer::sdk::GigPerformerAPI
{
  protected:
  public:
    // These must be here but no need to do anything unless you want extra behavior
    explicit LibMain(LibraryHandle handle)
        : GigPerformerAPI(handle)
    {
    }

    ~LibMain() override;

    void OnStatusChanged(GPStatusType status) override;

    void Initialization() override;

    int RequestGPScriptFunctionSignatureList(GPScript_AllowedLocations location,
                                             ExternalAPI_GPScriptFunctionDefinition **list) override;

    // This MUST be defined in your class
    string GetProductDescription() override;
};
