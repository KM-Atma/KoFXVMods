#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>

using namespace RC;

// -----------------------------------------------
// CONFIG
// Run cancel window.
// Original value: 13 (0x0D)
// Set to 1 for instant cancel, higher values for later cancel window.
// -----------------------------------------------
static constexpr uint8_t RUN_CANCEL_FRAME_LIMIT = 4;
static constexpr uintptr_t PATCH_OFFSET = 0x10BDA13;

class RunCancelMod : public CppUserModBase
{
private:
    uintptr_t m_patch_addr = 0;
    uint8_t   m_original_byte = 0;
    bool      m_patched = false;

public:
    RunCancelMod() : CppUserModBase()
    {
        ModName = STR("RunLock");
        ModVersion = STR("1.0");
        ModDescription = STR("Modifies the run cancel window");
        ModAuthors = STR("Atma");
    }

    ~RunCancelMod() override
    {
        if (m_patched && m_patch_addr)
        {
            DWORD old_protect;
            if (VirtualProtect(reinterpret_cast<void*>(m_patch_addr), 1, PAGE_EXECUTE_READWRITE, &old_protect))
            {
                *reinterpret_cast<uint8_t*>(m_patch_addr) = m_original_byte;
                VirtualProtect(reinterpret_cast<void*>(m_patch_addr), 1, old_protect, &old_protect);
            }
        }
    }

    auto on_unreal_init() -> void override
    {
        uintptr_t exe = reinterpret_cast<uintptr_t>(GetModuleHandleW(L"KOFXV-Win64-Shipping.exe"));
        if (!exe) return;

        m_patch_addr = exe + PATCH_OFFSET;
        m_original_byte = *reinterpret_cast<uint8_t*>(m_patch_addr);

        if (m_original_byte != 0x0D) return;

        DWORD old_protect;
        if (!VirtualProtect(reinterpret_cast<void*>(m_patch_addr), 1, PAGE_EXECUTE_READWRITE, &old_protect)) return;

        *reinterpret_cast<uint8_t*>(m_patch_addr) = RUN_CANCEL_FRAME_LIMIT;
        VirtualProtect(reinterpret_cast<void*>(m_patch_addr), 1, old_protect, &old_protect);

        m_patched = true;
    }

    auto on_update() -> void override {}
};

#define RUNCANCEL_API __declspec(dllexport)
extern "C"
{
    RUNCANCEL_API RC::CppUserModBase* start_mod() { return new RunCancelMod(); }
    RUNCANCEL_API void uninstall_mod(RC::CppUserModBase* mod) { delete mod; }
}