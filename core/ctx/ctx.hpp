#pragma once

// Macro to simplify getting module information
#define GET_MODULE(x, y) x = g_pe->get_module(y)

namespace csgo {
	// Context class for storing cheat initialization
	class c_ctx {
	public:
		// Initialize the context
		void initialize();

		// Get the modules stored in the context
		ALWAYS_INLINE const auto& modules() const { return m_modules; }

		ALWAYS_INLINE const auto& offsets() const { return m_offsets; }
	private:
		struct {
			// The data of the ntdll.dll
			pe::module_t m_ntdll{};
			
			// The data of the user32.dll
			pe::module_t m_user32{};

			// The data of the win32u.dll
			pe::module_t m_win32u{};

			// The data of the Winmm.dll
			pe::module_t m_Winmm{};
		} m_modules{};

		struct {
			uintptr_t m_gadget{};
		} m_offsets{};
	};
}
// Global instance of c_ctx
inline const auto g_ctx = std::make_unique<csgo::c_ctx>();