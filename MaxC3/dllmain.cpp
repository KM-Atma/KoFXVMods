#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <vector>
#include <string>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UFunction.hpp>

using namespace RC;
using namespace RC::Unreal;

// -----------------------------------------------
// CONFIG & OFFSETS
// -----------------------------------------------
static constexpr uintptr_t BASE_OFFSET = 0x05197880;
static constexpr uintptr_t OFF_MAX_MAXIMUM = 0x73A;

// -----------------------------------------------
// HELPERS
// -----------------------------------------------
static uintptr_t get_exe_base() {
    static uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"KOFXV-Win64-Shipping.exe"));
    return base;
}

template<typename T>
static T safe_read(uintptr_t addr) {
    __try { return *reinterpret_cast<T*>(addr); }
    __except (EXCEPTION_EXECUTE_HANDLER) { return T{}; }
}

template<typename T>
static void safe_write(uintptr_t addr, T value) {
    __try { *reinterpret_cast<T*>(addr) = value; }
    __except (EXCEPTION_EXECUTE_HANDLER) {}
}

static uintptr_t resolve_player(uintptr_t s1, uintptr_t s2, uintptr_t s3) {
    uintptr_t exe = get_exe_base();
    uintptr_t p = safe_read<uintptr_t>(exe + BASE_OFFSET);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + s1); if (!p) return 0;
    p = safe_read<uintptr_t>(p + s2); if (!p) return 0;
    p = safe_read<uintptr_t>(p + s3); if (!p) return 0;
    return p;
}

// -----------------------------------------------
// METER LOGIC
// -----------------------------------------------
static void update_max_logic() {
    uintptr_t players[2];
    players[0] = resolve_player(0x8, 0x8, 0x210);
    players[1] = resolve_player(0x98, 0x8, 0x20);

    for (int i = 0; i < 2; ++i) {
        if (!players[i]) continue;

        int32_t current_max_maximum = safe_read<int32_t>(players[i] + OFF_MAX_MAXIMUM);

        if (current_max_maximum == 1000 || current_max_maximum == 1250) {
            safe_write<int32_t>(players[i] + OFF_MAX_MAXIMUM, 1500);
        }
    }
}

class MaxMeterMod : public CppUserModBase {
public:
    MaxMeterMod() {
        ModName = STR("MaxC3");
        ModVersion = STR("1.2");
        ModDescription = STR("Sets max mode to 1500 across C1, C2, C3");
        ModAuthors = STR("Atma");
    }

    auto on_unreal_init() -> void override {
        Hook::RegisterProcessEventPreCallback([](UObject* obj, UFunction* fn, void* params) {
            if (fn && fn->GetFullName().find(STR("BP_GameModeBattle_C:ReceiveTick")) != std::wstring::npos) {
                update_max_logic();
            }
            });
    }

    ~MaxMeterMod() {}
};

#define MAXMOD_API __declspec(dllexport)
extern "C" {
    MAXMOD_API RC::CppUserModBase* start_mod() { return new MaxMeterMod(); }
    MAXMOD_API void uninstall_mod(RC::CppUserModBase* mod) { delete mod; }
}