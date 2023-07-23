#pragma once

struct address_t {
    ALWAYS_INLINE address_t() : m_address{ 0 } {}  // Default constructor

    ALWAYS_INLINE ~address_t() {}  // Destructor

    // Constructor from uintptr_t
    ALWAYS_INLINE address_t(uintptr_t a) : m_address{ a } {}

    // Constructor from const void*
    ALWAYS_INLINE address_t(const void* a) : m_address{ reinterpret_cast<uintptr_t>(a) } {}

    // Conversion operator to uintptr_t
    ALWAYS_INLINE operator uintptr_t() { return m_address; }

    // Conversion operator to void*
    ALWAYS_INLINE operator void* () { return reinterpret_cast<void*>(m_address); }

    // Conversion operator to const void*
    ALWAYS_INLINE operator const void* () { return reinterpret_cast<const void*>(m_address); }

    // Template function to cast the address to another type
    template <typename T = address_t>
    ALWAYS_INLINE T as() const {
        return (m_address) ? T(m_address) : T();
    }


    uintptr_t m_address{};
};