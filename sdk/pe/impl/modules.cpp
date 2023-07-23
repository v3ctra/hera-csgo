#include "../../sdk.hpp"

namespace pe {
	ALWAYS_INLINE bool is_module_valid(std::size_t base, PIMAGE_DOS_HEADER& out_dos, PIMAGE_NT_HEADERS& out_nt) {
		auto dos_headers = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
		if (dos_headers->e_magic != IMAGE_DOS_SIGNATURE)
			return false;

		auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(base + dos_headers->e_lfanew);
		if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
			return false;

		out_dos = dos_headers;
		out_nt = nt_headers;

		return true;
	}

	bool c_pe::get_all_modules(std::vector<module_t>& out) {
		IMAGE_DOS_HEADER* dos_header{};
		IMAGE_NT_HEADERS* nt_headers{};

		const auto peb = reinterpret_cast<PEB*>(__readfsdword(0x30));
		if (!peb)
			return false;

		//this is supremacy: list = &peb->Ldr->InMemoryOrderModuleList;
		//maybe this is why supremacy wasn't working for 90% users?
		auto list = peb->LdrData->InLoadOrderModuleList.Flink;
		for (list; list != &peb->LdrData->InLoadOrderModuleList; list = list->Flink) {
			_LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(list, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (!entry)
				continue;

			if (!is_module_valid(reinterpret_cast<std::size_t>(entry->DllBase), dos_header, nt_headers))
				continue;

			out.push_back({ entry, dos_header, nt_headers });
		}
		return !out.empty();
	}

	module_t c_pe::get_module(const uint32_t name) {
		std::vector<module_t> modules{};

		// get all modules.
		if (!get_all_modules(modules))
			return {};

		// iterate all modules.
		for (const auto& m : modules) {
			if (hash(m.get_name().c_str()) == name)
				return m;
		}

		return {};
	}

	/* This function isn't reliable as it doesn't support forwarded calls. */
	address_t c_pe::get_export(module_t module, uint32_t hash, bool in_memory) {
		auto rva_2_offset = [](std::uint32_t rva, PIMAGE_NT_HEADERS nt_headers, bool in_memory = false) {
			if (!rva || !in_memory)
				return static_cast<std::int32_t>(rva);

			auto sec = IMAGE_FIRST_SECTION(nt_headers);
			for (size_t i = 0; i < nt_headers->FileHeader.NumberOfSections; i++) {
				if (rva >= sec->VirtualAddress && rva < sec->VirtualAddress + sec->Misc.VirtualSize)
					break;
				sec++;
			}
			return static_cast<std::int32_t>(rva - sec->VirtualAddress + sec->PointerToRawData);
		};

		auto base = module.get_base();
		auto nt_headers = module.get_nt_header();

		auto exports = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
			rva_2_offset(base + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
				nt_headers, in_memory));

		auto names = reinterpret_cast<std::uint32_t*>(base + rva_2_offset(exports->AddressOfNames, nt_headers, in_memory));

		auto ordinal_index = static_cast<std::uint32_t>(-1);

		// go thru all exported functions until we find our export
		for (std::uint32_t i = 0; i < exports->NumberOfFunctions; i++) {
			auto function_name = reinterpret_cast<const char*>(base + rva_2_offset(names[i], nt_headers, in_memory));

			// it is what it is
			if (hash(function_name) == hash) {
				ordinal_index = i;
				break;
			}
		}

		if (ordinal_index > exports->NumberOfFunctions)
			return { };

		auto ordinals = reinterpret_cast<std::uint16_t*>(base + rva_2_offset(exports->AddressOfNameOrdinals, nt_headers, in_memory));
		auto addresses = reinterpret_cast<std::uint32_t*>(base + rva_2_offset(exports->AddressOfFunctions, nt_headers, in_memory));

		return base + rva_2_offset(addresses[ordinals[ordinal_index]], nt_headers, in_memory);
	}
}