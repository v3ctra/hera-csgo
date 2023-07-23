#pragma once
#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include <string>

namespace fnv {
	constexpr uint32_t offsetBasis = 0x811c9dc5;
	constexpr uint32_t prime = 0x1000193;

	constexpr uint32_t hash(char const* string, const uint32_t val = offsetBasis) noexcept {
		return (string[0] == '\0') ? val : hash(&string[1], (val ^ uint32_t(string[0])) * prime);
	}

	constexpr uint32_t hash(wchar_t const* string, const uint32_t val = offsetBasis) noexcept {
		return (string[0] == L'\0') ? val : hash(&string[1], (val ^ uint32_t(string[0])) * prime);
	}

	constexpr uint32_t hashRuntime(const char* str) noexcept
	{
		auto value = offsetBasis;

		while (*str) {
			value ^= *str++;
			value *= prime;
		}
		return value;
	}
}

#define hash(x) fnv::hash(x)
#endif