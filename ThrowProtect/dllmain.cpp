#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <MinHook.h>
#include <Mod/CppUserModBase.hpp>
#include <DynamicOutput/DynamicOutput.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/UFunction.hpp>
#include <unordered_map>
#include <vector>

using namespace RC;
using namespace RC::Unreal;

// -----------------------------------------------
// CONFIG
// -----------------------------------------------
static constexpr int32_t KNOCKDOWN_STATE_1 = 53;
static constexpr int32_t KNOCKDOWN_STATE_2 = 49;
static constexpr int32_t NORMAL_THROW_STATE = 37;
static constexpr int32_t RECOVERY_WINDOW_FRAMES = 10;
static constexpr uintptr_t HITBOX_ID_OFFSET = 0x2A;
static constexpr uintptr_t HOOK_OFFSET = 0x108BB60;

static const std::unordered_map<int32_t, std::vector<uint16_t>> NORMAL_THROW_IDS = {
    {  0, { 47, 48 } },  // Kyo Kusanagi
    {  1, { 57, 58 } },  // Benimaru Nikaido
    {  3, { 57, 58 } },  // Ryo Sakazaki
    {  4, { 57, 58 } },  // Robert Garcia
    {  5, { 57, 58 } },  // Yuri Sakazaki
    {  6, { 57, 58 } },  // Terry Bogard
    {  7, { 57, 58 } },  // Andy Bogard
    {  8, { 57, 58 } },  // Joe Higashi
    {  9, { 57, 58 } },  // Leona Heidern
    { 10, { 57, 58 } },  // Ralf Jones
    { 11, { 58, 59 } },  // Clark Still
    { 12, { 57, 58 } },  // Iori Yagami
    { 13, { 59, 60 } },  // Mature
    { 14, { 57, 58 } },  // Vice
    { 15, { 57, 58 } },  // K'
    { 16, { 57, 58 } },  // Kula Diamond
    { 17, { 57, 58 } },  // Maxima
    { 18, { 60, 61 } },  // Geese Howard
    { 19, { 58, 59 } },  // Billy Kane
    { 21, { 57, 58 } },  // King
    { 22, { 57, 58 } },  // Mai Shiranui
    { 24, { 58, 59 } },  // Kim Kaphwan
    { 26, { 57, 58 } },  // Luong
    { 27, { 57, 58 } },  // Athena Asamiya
    { 33, { 59, 60 } },  // Nakoruru
    { 36, { 57, 58 } },  // Ramón
    { 37, { 57, 58 } },  // Ángel
    { 38, { 57, 58 } },  // King of Dinosaurs
    { 39, { 58, 59 } },  // Shun'Ei
    { 41, { 57, 58 } },  // Meitenkun
    { 45, { 57, 58 } },  // Sylvie Paula Paula
    { 46, { 57, 58 } },  // Kukri
    { 48, { 57, 58 } },  // Antonov
    { 50, { 57, 58 } },  // Rock Howard
    { 51, { 57, 58 } },  // Blue Mary
    { 52, { 57, 58 } },  // Ryuji Yamazaki
    { 53, { 57, 58 } },  // Vanessa
    { 56, { 57, 58 } },  // Heidern
    { 57, { 57, 58 } },  // Whip
    { 60, { 59, 60 } },  // Najd
    { 61, { 59, 60 } },  // Isla
    { 62, { 58, 59 } },  // Dolores
    { 63, { 57, 58 } },  // Chizuru Kagura
    { 64, { 61, 62 } },  // Ash Crimson
    { 65, { 61, 62 } },  // Elisabeth Blanctorche
    { 66, { 60, 61 } },  // Krohnen
    { 67, { 58, 59 } },  // Yashiro Nanakase
    { 68, { 56, 57 } },  // Shermie
    { 69, { 57, 58 } },  // Chris
    { 70, { 58, 59 } },  // B. Jenet
    { 71, { 58, 59 } },  // Gato
    { 72, { 58, 59 } },  // Orochi Yashiro
    { 73, { 56, 57 } },  // Orochi Shermie
    { 74, { 57, 58 } },  // Orochi Chris
    { 75, { 58, 59 } },  // Haohmaru
    { 77, { 57, 58 } },  // Darli Dagger
    { 78, { 61, 62 } },  // Omega Rugal
    { 79, { 58, 59 } },  // Shingo Yabuki
    { 80, { 59, 60 } },  // Hinako Shijou
    { 81, { 58, 59 } },  // Duo Lon
    { 82, { 58, 59 } },  // Goenitz
};

// -----------------------------------------------
// CE pointer chain constants
// -----------------------------------------------
static constexpr uintptr_t EXE_OFFSET = 0x05197880;

static constexpr uintptr_t P1_STEP1 = 0x8;
static constexpr uintptr_t P1_STEP2 = 0x8;
static constexpr uintptr_t P1_STEP3 = 0x210;

static constexpr uintptr_t P2_STEP1 = 0x98;
static constexpr uintptr_t P2_STEP2 = 0x8;
static constexpr uintptr_t P2_STEP3 = 0x20;

static constexpr uintptr_t OFFSET_STATE = 0x1A0;
static constexpr uintptr_t OFFSET_CHAR_PTR = 0x118;
static constexpr uintptr_t OFFSET_CHAR_ID = 0x30;

// -----------------------------------------------
// Memory helpers
// -----------------------------------------------
template<typename T>
static T safe_read(uintptr_t addr)
{
    __try { return *reinterpret_cast<T*>(addr); }
    __except (EXCEPTION_EXECUTE_HANDLER) { return T{}; }
}

// -----------------------------------------------
// Pointer chain resolvers
// -----------------------------------------------
static uintptr_t get_exe_base()
{
    return reinterpret_cast<uintptr_t>(GetModuleHandleW(L"KOFXV-Win64-Shipping.exe"));
}

static uintptr_t resolve_p1_base()
{
    uintptr_t exe = get_exe_base();
    if (!exe) return 0;
    uintptr_t p = safe_read<uintptr_t>(exe + EXE_OFFSET);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + P1_STEP1);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + P1_STEP2);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + P1_STEP3);
    if (!p) return 0;
    return p;
}

static uintptr_t resolve_p2_base()
{
    uintptr_t exe = get_exe_base();
    if (!exe) return 0;
    uintptr_t p = safe_read<uintptr_t>(exe + EXE_OFFSET);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + P2_STEP1);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + P2_STEP2);
    if (!p) return 0;
    p = safe_read<uintptr_t>(p + P2_STEP3);
    if (!p) return 0;
    return p;
}

static int32_t get_char_id(uintptr_t player_base)
{
    return safe_read<int32_t>(player_base + OFFSET_CHAR_PTR + OFFSET_CHAR_ID);
}

static bool is_knockdown(int32_t state)
{
    return state == KNOCKDOWN_STATE_1 || state == KNOCKDOWN_STATE_2;
}

static bool is_normal_throw(int32_t char_id, uint16_t hitbox_id)
{
    auto it = NORMAL_THROW_IDS.find(char_id);
    if (it == NORMAL_THROW_IDS.end()) return false;
    for (auto id : it->second)
        if (id == hitbox_id) return true;
    return false;
}

// -----------------------------------------------
// Global state
// -----------------------------------------------
static int32_t g_p1_state_last = -1;
static int32_t g_p2_state_last = -1;
static int32_t g_p1_recovery_frames = 0;
static int32_t g_p2_recovery_frames = 0;

typedef void(*HitboxCopyFn)(void* rcx, void* rdx);
static HitboxCopyFn g_original_hitbox_copy = nullptr;

// -----------------------------------------------
// MinHook detour
// -----------------------------------------------
static void hooked_hitbox_copy(void* rcx, void* rdx)
{
    uint16_t hitbox_id = safe_read<uint16_t>(
        reinterpret_cast<uintptr_t>(rdx) + HITBOX_ID_OFFSET
    );

    uintptr_t p1 = resolve_p1_base();
    uintptr_t p2 = resolve_p2_base();

    if (p1 && p2)
    {
        int32_t p1_state = safe_read<int32_t>(p1 + OFFSET_STATE);
        int32_t p2_state = safe_read<int32_t>(p2 + OFFSET_STATE);
        int32_t p1_char = get_char_id(p1);
        int32_t p2_char = get_char_id(p2);

        // Block P1 normal throw during knockdown or recovery window
        if (p1_state == NORMAL_THROW_STATE &&
            (is_knockdown(g_p1_state_last) ||
                g_p1_recovery_frames > 0) &&
            is_normal_throw(p1_char, hitbox_id))
        {
            Output::send<LogLevel::Verbose>(
                STR("[ThrowProtect] Blocked P1 normal throw hitbox {} (last={} rec={})\n"),
                hitbox_id, g_p1_state_last, g_p1_recovery_frames);
            return;
        }

        // Block P2 normal throw during knockdown or recovery window
        if (p2_state == NORMAL_THROW_STATE &&
            (is_knockdown(g_p2_state_last) ||
                g_p2_recovery_frames > 0) &&
            is_normal_throw(p2_char, hitbox_id))
        {
            Output::send<LogLevel::Verbose>(
                STR("[ThrowProtect] Blocked P2 normal throw hitbox {} (last={} rec={})\n"),
                hitbox_id, g_p2_state_last, g_p2_recovery_frames);
            return;
        }
    }

    g_original_hitbox_copy(rcx, rdx);
}

// -----------------------------------------------
// State tracking
// -----------------------------------------------
static void update_state_tracking()
{
    uintptr_t p1 = resolve_p1_base();
    uintptr_t p2 = resolve_p2_base();

    if (!p1 || !p2)
    {
        g_p1_state_last = -1;
        g_p2_state_last = -1;
        g_p1_recovery_frames = 0;
        g_p2_recovery_frames = 0;
        return;
    }

    int32_t p1_state = safe_read<int32_t>(p1 + OFFSET_STATE);
    int32_t p2_state = safe_read<int32_t>(p2 + OFFSET_STATE);

    if (p1_state != g_p1_state_last)
    {
        if (is_knockdown(p1_state))
        {
            g_p1_recovery_frames = 0;
            Output::send<LogLevel::Verbose>(STR("[ThrowProtect] P1 entered knockdown (state={})\n"), p1_state);
        }
        else if (is_knockdown(g_p1_state_last))
        {
            g_p1_recovery_frames = RECOVERY_WINDOW_FRAMES + 1;
            Output::send<LogLevel::Verbose>(STR("[ThrowProtect] P1 recovered, window started ({} frames)\n"), RECOVERY_WINDOW_FRAMES);
        }
    }

    if (p2_state != g_p2_state_last)
    {
        if (is_knockdown(p2_state))
        {
            g_p2_recovery_frames = 0;
            Output::send<LogLevel::Verbose>(STR("[ThrowProtect] P2 entered knockdown (state={})\n"), p2_state);
        }
        else if (is_knockdown(g_p2_state_last))
        {
            g_p2_recovery_frames = RECOVERY_WINDOW_FRAMES + 1;
            Output::send<LogLevel::Verbose>(STR("[ThrowProtect] P2 recovered, window started ({} frames)\n"), RECOVERY_WINDOW_FRAMES);
        }
    }

    if (g_p1_recovery_frames > 0) g_p1_recovery_frames--;
    if (g_p2_recovery_frames > 0) g_p2_recovery_frames--;

    g_p1_state_last = p1_state;
    g_p2_state_last = p2_state;
}

// -----------------------------------------------
// Mod class
// -----------------------------------------------
class ThrowProtectMod : public CppUserModBase
{
public:
    ThrowProtectMod() : CppUserModBase()
    {
        ModName = STR("ThrowProtect");
        ModVersion = STR("1.0");
        ModDescription = STR("Blocks normal throws on recovery");
        ModAuthors = STR("Atma");
    }

    ~ThrowProtectMod() override
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
    }

    auto on_unreal_init() -> void override
    {
        Hook::RegisterProcessEventPreCallback([](UObject* obj, UFunction* fn, void* params)
            {
                if (!fn || !obj) return;
                if (fn->GetFullName().find(STR("BP_GameModeBattle_C:ReceiveTick")) != std::wstring::npos)
                    update_state_tracking();
            });

        if (MH_Initialize() != MH_OK)
        {
            Output::send<LogLevel::Error>(STR("[ThrowProtect] MinHook init failed\n"));
            return;
        }

        uintptr_t hook_addr = get_exe_base() + HOOK_OFFSET;

        if (MH_CreateHook(
            reinterpret_cast<void*>(hook_addr),
            reinterpret_cast<void*>(&hooked_hitbox_copy),
            reinterpret_cast<void**>(&g_original_hitbox_copy)) != MH_OK)
        {
            Output::send<LogLevel::Error>(STR("[ThrowProtect] Failed to create hitbox hook\n"));
            return;
        }

        if (MH_EnableHook(reinterpret_cast<void*>(hook_addr)) != MH_OK)
        {
            Output::send<LogLevel::Error>(STR("[ThrowProtect] Failed to enable hitbox hook\n"));
            return;
        }

        Output::send<LogLevel::Verbose>(STR("[ThrowProtect] Mod initialised, hitbox hook active\n"));
    }

    auto on_update() -> void override {}
};

// -----------------------------------------------
// Entry points
// -----------------------------------------------
#define THROWPROTECT_API __declspec(dllexport)
extern "C"
{
    THROWPROTECT_API RC::CppUserModBase* start_mod() { return new ThrowProtectMod(); }
    THROWPROTECT_API void uninstall_mod(RC::CppUserModBase* mod) { delete mod; }
}