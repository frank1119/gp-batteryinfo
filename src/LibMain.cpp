#include "LibMain.h"

using GPUtils = gigperformer::sdk::GPUtils;
using namespace std;


/// <summary>
/// Holds the constructed object.
/// Provides a way to execute GP_Functions like consoleLog(..)
/// </summary>
LibMain *thisObj = nullptr;

/// Ignore a given value
/// \details this is a dummy function to suppress compiler warnings about unused parameters
template <typename T>
void Ignore(T const &) noexcept
{
}

// define an XML string describing your product
const string XMLProductDescription =
    // Replace with your information
    "<Library>"
    "  <Product"
    "    Name=\"BatteryInfo\""
    "    Version=\"1.0\""
    "    BuildDate=\"01/14/2023\">"
    "  </Product> "
    "  <Description>Extension for getting Battery Info. (c) 2023 F. den Blaauwen</Description>"
    "  <ImagePath>/Path/To/ImageFile/foo.jpg</ImagePath>"
    "</Library>";



LibMain::~LibMain()
{
    thisObj = nullptr;
}

#pragma region Events and housekeeping
void LibMain::OnStatusChanged(GPStatusType status)
{
}

void LibMain::Initialization()
{
    // Dirty way to make some instance methods, like consoleLog() available for
    // library-defined script functions
    thisObj = this;
    // Do any initialization that you need

    // .... your code here

    // Finally, register all the methods that you are going to actually use,
    // i.e, the ones you declared above as override
    registerCallback("OnStatusChanged");
}

string LibMain::GetProductDescription()
{
    // Generally don't touch this - simply define the constant
    // 'XMLProductDescription' at the top of this file with an XML description of
    // your product
    return XMLProductDescription;
}
#pragma endregion


#pragma region Get Values
extern "C" void GPGetACLineStatus(GPRuntimeEngine *vm)
{
    GP_VM_PushInteger(vm, GetACLineStatus());
}

extern "C" void GPGetBatteryPercent(GPRuntimeEngine *vm)
{
    GP_VM_PushInteger(vm, GetBatteryPercent());
}

extern "C" void GPGetBatteryLifeTime(GPRuntimeEngine *vm)
{
    GP_VM_PushInteger(vm, GetBatteryLifeTime());
}

extern "C" void GPGetBatteryCharging(GPRuntimeEngine *vm)
{
    GP_VM_PushBoolean(vm, GetBatteryCharging() != 0);
}

extern "C" void GPGetBatteryDischarging(GPRuntimeEngine *vm)
{
    GP_VM_PushBoolean(vm, GetBatteryDischarging()!=0);
}

extern "C" void GPGetBatteryPresent(GPRuntimeEngine *vm)
{
    GP_VM_PushBoolean(vm, GetBatteryPresent()!=0);
}
#pragma endregion


#pragma region Definitions

ExternalAPI_GPScriptFunctionDefinition functionList[] = {
    {"GetACLineStatus", "", "Returns integer", "Returns the current linestate", GPGetACLineStatus},
    {"GetBatteryPercent", "", "Returns integer", "Returns the percentage the battery is charged", GPGetBatteryPercent},
    {"GetBatteryCharging", "", "Returns boolean", "Returns true when the battery is charging", GPGetBatteryCharging},
    {"GetBatteryDischarging", "", "Returns boolean", "Returns true when the battery is discharging", GPGetBatteryDischarging},
    {"GetBatteryPresent", "", "Returns boolean", "Returns true when the battery is present", GPGetBatteryPresent},
    {"GetBatteryLifeTime", "", "Returns integer", "Returns the estimated time left", GPGetBatteryLifeTime},
};

int LibMain::RequestGPScriptFunctionSignatureList(GPScript_AllowedLocations location,
                                                  ExternalAPI_GPScriptFunctionDefinition **list)
{
    Ignore(location);
    // Allow these in ANY script so no need to check the location field
    *list = functionList;
    int count = sizeof(functionList) / sizeof(ExternalAPI_GPScriptFunctionDefinition);
    return count;
}

#pragma endregion

namespace gigperformer
{
namespace sdk
{

GigPerformerAPI *CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}

} // namespace sdk
} // namespace gigperformer
