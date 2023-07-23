#pragma once
#ifndef SUPRA_SDK_PATTERN_HPP
#define SUPRA_SDK_PATTERN_HPP

class pattern {
public:
    /*
    static address_t search(memory::module_t mod, const char* signature) {
        /*
        static auto pattern_to_byte_legacy = [](const char* pattern) {
            std::vector<int> bytes;

            // Iterate through the pattern string
            for (const char* p = pattern; *p != '\0'; p += 2) {
                // Extract the next two characters as a hex string
                char hex[3] = { p[0], p[1], '\0' };

                // Convert the hex string to an integer and push it to the bytes vector
                if (hex[0] != '?' && hex[1] != '?') {
                    bytes.push_back(std::stoi(hex, nullptr, 16));
                }
                else {
                    bytes.push_back(-1);
                }
            }

            return bytes;
        };

        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        // Get the size of the module's image (i.e., the size of its memory space)
        auto size_of_image = mod.nt()->OptionalHeader.SizeOfImage;

        // Convert the signature string into a vector of bytes
        auto pattern_bytes = pattern_to_byte(signature);

        // Get a pointer to the start of the module's memory space
        auto scan_bytes = reinterpret_cast<std::uint8_t*>(mod.base());

        // Get the size and data of the signature vector
        auto s = pattern_bytes.size();
        auto d = pattern_bytes.data();

        // Iterate through the module's memory space, checking for matches to the signature
        for (auto i = 0u; i < static_cast<unsigned long>(size_of_image - s); ++i) {
            bool found = true;
            for (auto j = 0u; j < s; ++j) {
                // Check if the byte at the current position matches the corresponding byte in the signature
                // If the signature byte is a wildcard, skip the comparison
                if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if (found) {
                // Return a pointer to the start of the matching bytes in the module's memory space
                return &scan_bytes[i];
            }
        }

        // If no match was found, return a null pointer
        return {};
    }
*/
//nie pytajcie...
    static address_t search(pe::module_t mod, const char* signature) {
        static auto pattern_to_byte = [](std::string pattern) {
            std::vector<int> bytes{};

            // Loops over each character in the string.
            for (auto it = pattern.begin(); it != pattern.end(); ++it) {
                if (*it == '?') {
                    bytes.push_back(-1);
                    continue;
                }

                // Parse the next two characters as a hex byte string and push the result.
                if (std::isxdigit(*it) && std::isxdigit(*(it + 1))) {
                    std::string byte_str = { it, it + 2 };
                    bytes.push_back(std::stoi(byte_str, nullptr, 16));
                    ++it;
                }
            }
            /*
            // Loops over each character in the string.
            for (auto it = pattern.begin(); it != pattern.end(); it++) {
                // If the character is a question mark, push -1 to the `bytes` vector and continue to the next character.
                if (*it == '?') {
                    bytes.push_back(-1);
                    continue;
                }

                // If character or the next next isn't hex digit, skip it
                if (!std::isxdigit(*it) || !std::isxdigit(*(it + 1))) {
                    continue;
                }

                // Parse the next two characters as a hex byte string and push the result.
                auto byte_str = std::string{ it, it + 2 };
                bytes.push_back(std::stoi(byte_str, nullptr, 16));
                it++;
            }*/

            return bytes;
        };

        // Convert the signature string into a vector of bytes
        auto pattern_bytes = pattern_to_byte(signature);

        // Get the size of the module's image (i.e., the size of its memory space)
        auto size_of_image = mod.get_nt_header()->OptionalHeader.SizeOfImage;

        // Get a pointer to the start of the module's memory space
        auto* scan_bytes = reinterpret_cast<std::uint8_t*>(mod.get_base());

        // Search for the pattern
        auto* result = std::search(scan_bytes, scan_bytes + size_of_image - pattern_bytes.size(),
            pattern_bytes.begin(), pattern_bytes.end(), [](auto a, auto b) {
                return (a == b) || (b == -1);
        });

        // Return a pointer to the start of the matching bytes in the module's memory space, or 0 if not found
        return (result != scan_bytes + size_of_image - pattern_bytes.size()) ? result : 0;
    }
};

#define BYTESEQ(in, bytes_sequence) pattern::search(in, bytes_sequence)
#endif