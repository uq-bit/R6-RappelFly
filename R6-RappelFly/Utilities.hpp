#pragma once

// C++ Headers
#include <memory>

// API Headers
#include <Windows.h>
#include <winternl.h>

namespace utilities
{
	template<typename DefaultTy = std::uintptr_t>
	[[nodiscard]] DefaultTy imageBase()
	{
		return (DefaultTy)NtCurrentTeb()->ProcessEnvironmentBlock->Reserved3[1];
	}
}