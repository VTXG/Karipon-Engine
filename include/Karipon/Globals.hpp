#pragma once

#if TARGET_KRP
#define IF_TARGET_KRP(...) __VA_ARGS__
#define IF_TARGET_KRP_COND(cond) if (cond)
#define IF_TARGET_KRP_ARG(expr) , expr
#define IF_NOT_TARGET_KRP(...)
#define TARGET_KRP_IF_ELSE(TARGET_KRP, orig) TARGET_KRP
#else
#define IF_TARGET_KRP(...)
#define IF_TARGET_KRP_ARG(expr)
#define IF_TARGET_KRP_COND(cond)
#define IF_NOT_TARGET_KRP(...) __VA_ARGS__
#define TARGET_KRP_IF_ELSE(TARGET_KRP, orig) orig
#endif
