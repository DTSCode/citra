// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common/common_types.h"
#include "core/mem_map.h"
#include "core/hle/hle.h"

namespace HLE {

#define PARAM(n)    Core::g_app_core->GetReg(n)

/**
 * HLE a function return from the current ARM11 userland process
 * @param res Result to return
 */
static inline void FuncReturn(u32 res) {
    Core::g_app_core->SetReg(0, res);
}

/**
 * HLE a function return (64-bit) from the current ARM11 userland process
 * @param res Result to return (64-bit)
 * @todo Verify that this function is correct
 */
static inline void FuncReturn64(u64 res) {
    Core::g_app_core->SetReg(0, (u32)(res & 0xFFFFFFFF));
    Core::g_app_core->SetReg(1, (u32)((res >> 32) & 0xFFFFFFFF));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function wrappers that return type s32

template<s32 func(u32, u32, u32, u32)> void Wrap() {
    FuncReturn(func(PARAM(0), PARAM(1), PARAM(2), PARAM(3)));
}

template<s32 func(u32, u32, u32, u32, u32)> void Wrap() {
    FuncReturn(func(PARAM(0), PARAM(1), PARAM(2), PARAM(3), PARAM(4)));
}

template<s32 func(u32*, u32, u32, u32, u32, u32)> void Wrap(){
    u32 param_1 = 0;
    u32 retval = func(&param_1, PARAM(0), PARAM(1), PARAM(2), PARAM(3), PARAM(4));
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

template<s32 func(s32*, u32*, s32, bool, s64)> void Wrap() {
    s32 param_1 = 0;
    s32 retval = func(&param_1, (Handle*)Memory::GetPointer(PARAM(1)), (s32)PARAM(2),
        (PARAM(3) != 0), (((s64)PARAM(4) << 32) | PARAM(0)));
    Core::g_app_core->SetReg(1, (u32)param_1);
    FuncReturn(retval);
}

// TODO(bunnei): Is this correct? Probably not - Last parameter looks wrong for ArbitrateAddress
template<s32 func(u32, u32, u32, u32, s64)> void Wrap() {
    FuncReturn(func(PARAM(0), PARAM(1), PARAM(2), PARAM(3), (((s64)PARAM(5) << 32) | PARAM(4))));
}

template<s32 func(u32*)> void Wrap(){
    u32 param_1 = 0;
    u32 retval = func(&param_1);
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

template<s32 func(u32, s64)> void Wrap() {
    FuncReturn(func(PARAM(0), (((s64)PARAM(3) << 32) | PARAM(2))));
}

template<s32 func(void*, void*, u32)> void Wrap(){
    FuncReturn(func(Memory::GetPointer(PARAM(0)), Memory::GetPointer(PARAM(1)), PARAM(2)));
}

template<s32 func(s32*, u32)> void Wrap(){
    s32 param_1 = 0;
    u32 retval = func(&param_1, PARAM(1));
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

template<s32 func(u32, s32)> void Wrap() {
    FuncReturn(func(PARAM(0), (s32)PARAM(1)));
}

template<s32 func(u32*, u32)> void Wrap(){
    u32 param_1 = 0;
    u32 retval = func(&param_1, PARAM(1));
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

template<s32 func(u32)> void Wrap() {
    FuncReturn(func(PARAM(0)));
}

template<s32 func(void*)> void Wrap() {
    FuncReturn(func(Memory::GetPointer(PARAM(0))));
}

template<s32 func(s64*, u32, void*, s32)> void Wrap(){
    FuncReturn(func((s64*)Memory::GetPointer(PARAM(0)), PARAM(1), Memory::GetPointer(PARAM(2)),
        (s32)PARAM(3)));
}

template<s32 func(u32*, const char*)> void Wrap() {
    u32 param_1 = 0;
    u32 retval = func(&param_1, Memory::GetCharPointer(PARAM(1)));
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

template<s32 func(u32*, s32, s32)> void Wrap() {
    u32 param_1 = 0;
    u32 retval = func(&param_1, PARAM(1), PARAM(2));
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

template<s32 func(s32*, u32, s32)> void Wrap() {
    s32 param_1 = 0;
    u32 retval = func(&param_1, PARAM(1), PARAM(2));
    Core::g_app_core->SetReg(1, param_1);
    FuncReturn(retval);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function wrappers that return type u32

template<u32 func()> void Wrap() {
    FuncReturn(func());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function wrappers that return type s64

template<s64 func()> void Wrap() {
    FuncReturn64(func());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Function wrappers that return type void

template<void func(s64)> void Wrap() {
    func(((s64)PARAM(1) << 32) | PARAM(0));
}

template<void func(const char*)> void Wrap() {
    func(Memory::GetCharPointer(PARAM(0)));
}

#undef PARAM
#undef FuncReturn

} // namespace HLE
