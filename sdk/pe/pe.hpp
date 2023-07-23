#pragma once
#include "detail/address.hpp"
#include "detail/structs.hpp"

namespace pe {
	// Represents a module loaded in the cheat process
	struct module_t {
		ALWAYS_INLINE module_t() :
			m_ldr{ nullptr }, m_dos{ nullptr }, m_nt{ nullptr } { }

		ALWAYS_INLINE module_t(LDR_DATA_TABLE_ENTRY* ldr_entry, IMAGE_DOS_HEADER* dos, IMAGE_NT_HEADERS* nt) :
			m_ldr{ ldr_entry }, m_dos{ dos }, m_nt{ nt } { }

		// Get the base address of the module
		ALWAYS_INLINE std::size_t get_base() const {
			return (m_ldr) ? address_t(m_ldr->DllBase) : address_t{ };
		}

		// Get the NT headers of the module
		ALWAYS_INLINE IMAGE_NT_HEADERS* get_nt_header() const {
			return (m_nt) ? m_nt : nullptr;
		}

		// Conversion operator to check if the module is valid
		ALWAYS_INLINE operator bool() const {
			return get_base();
		}

		// Logical negation operator to check if the module is invalid
		ALWAYS_INLINE bool operator !() const {
			return !get_base();
		}

		// Get the name of the module
		ALWAYS_INLINE std::string get_name() const {
			std::string out;

			if (!m_ldr)
				return { };

			auto dllName = reinterpret_cast<UNICODE_STRING*>(&m_ldr->BaseDllName);
			auto wide_name = std::wstring(dllName->Buffer, dllName->Length >> 1);

			out = std::string(wide_name.begin(), wide_name.end());
			std::ranges::transform(out.begin(), out.end(), out.begin(), ::tolower);
			return out;
		}

		LDR_DATA_TABLE_ENTRY* m_ldr{};    // Pointer to the LDR_DATA_TABLE_ENTRY structure
		IMAGE_DOS_HEADER* m_dos{};        // Pointer to the IMAGE_DOS_HEADER structure
		IMAGE_NT_HEADERS* m_nt{};         // Pointer to the IMAGE_NT_HEADERS structure
	};

	class c_pe {
	public:
		// Get information about all loaded modules
		bool get_all_modules(std::vector<module_t>& out);

		// Get information about a specific module by name hash
		module_t get_module(const uint32_t name);

		// Get the address of an exported function from a module
		address_t get_export(module_t module, uint32_t hash, bool in_memory = false);
	};
}

// Global instance of the c_pe class
inline const auto g_pe = std::make_unique< pe::c_pe >();

#include "detail/pattern.hpp"