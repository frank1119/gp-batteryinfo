#pragma once

#include "gigperformer/sdk/GPMidiMessages.h"
#include "gigperformer/sdk/GPUtils.h"
#include "gigperformer/sdk/GigPerformerAPI.h"
#include "gigperformer/sdk/types.h"
#include <windows.h>
#include <powerbase.h>

using namespace std;

/// <summary>
/// Global, because one fits all
/// </summary>
SYSTEM_BATTERY_STATE sbs;
SYSTEM_POWER_STATUS sps;


/// <summary>
/// Retrieves Power State and Battery State
/// </summary>
void RefreshState()
{
    // This is on purpose.
    CallNtPowerInformation((POWER_INFORMATION_LEVEL)5, NULL, 0, &sbs, sizeof(sbs));
    CallNtPowerInformation((POWER_INFORMATION_LEVEL)5, NULL, 0, &sbs, sizeof(sbs));
    GetSystemPowerStatus(&sps);
}

/// <summary>
/// Returns the AC line state
/// </summary>
/// <returns>0 -> No AC, 1 -> AC connected, 255 -> Unknown</returns>
int GetACLineStatus()
{
    RefreshState();
    return sps.ACLineStatus;
}

/// <summary>
/// The percentage the battery is charged
/// </summary>
/// <returns>The percentage</returns>
int GetBatteryPercent()
{
    RefreshState();
    return sps.BatteryLifePercent;
}

/// <summary>
/// Is the battery charging. If not, this doest not 
/// imply the battery is discharging!
/// </summary>
/// <returns>True when charging</returns>
int GetBatteryCharging()
{
    RefreshState();
    return sbs.Charging;
}

/// <summary>
/// Is the battery discharging. If not, this doest not 
/// imply the battery is charging!
/// </summary>
/// <returns>True when discharging</returns>
int GetBatteryDischarging()
{
    RefreshState();
    return sbs.Discharging;
}

/*
/// <summary>
/// The current (in mA) the battery is being charged or discharged (positive/negative)
/// </summary>
/// <returns>The rate in mA</returns>
long GetRate()
{
    RefreshState();
    return sbs.Rate;
}
*/

/// <summary>
/// Tells whether a battery is present or not
/// </summary>
/// <returns>True if present</returns>
int GetBatteryPresent()
{
    RefreshState();
    return sbs.BatteryPresent;
}

/// <summary>
/// Gets the estimated remaining lifetime in seconds
/// </summary>
/// <returns>The estimated remaining lifetime in seconds</returns>
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
