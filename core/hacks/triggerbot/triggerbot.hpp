#pragma once

namespace hacks {
	class c_triggerbot {
	public:
		void run() const;
	};
}
// Global instance of c_triggerbot
inline const auto g_triggerbot = std::make_unique< hacks::c_triggerbot >();