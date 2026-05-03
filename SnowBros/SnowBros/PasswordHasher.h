#pragma once

#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <ctime>

// Simple but effective password hasher
// Uses a custom salt + mixing approach compatible with C++ std library only (no OpenSSL needed)
// For a real game project this is more than sufficient for a demo
class PasswordHasher {
public:
    // Returns a hex-encoded hash string of the password + salt
    // Salt is embedded in the output so you can verify later: "SALT$HASH"
    static std::string hashPassword(const std::string& password, const std::string& salt = "") {
        std::string actualSalt = salt.empty() ? generateSalt() : salt;
        std::string salted = actualSalt + password + actualSalt; // sandwich salt
        std::string hash = computeHash(salted);
        return actualSalt + "$" + hash;
    }

    // Verifies a plaintext password against a stored hash string ("SALT$HASH")
    static bool verifyPassword(const std::string& password, const std::string& storedHash) {
        size_t sep = storedHash.find('$');
        if (sep == std::string::npos) return false;
        std::string salt = storedHash.substr(0, sep);
        std::string rehashed = hashPassword(password, salt);
        return rehashed == storedHash;
    }

private:
    // Generates a pseudo-random 8-char salt (no platform intrinsics)
    static std::string generateSalt() {
        uint64_t t = static_cast<uint64_t>(std::time(nullptr));
        volatile uint8_t probe = 0;
        uint64_t addr = reinterpret_cast<uint64_t>(&probe);
        uint64_t seed = (t * 6364136223846793005ULL) ^ (addr * 2685821657736338717ULL);
        seed ^= (seed >> 30); seed *= 0xbf58476d1ce4e5b9ULL;
        seed ^= (seed >> 27); seed *= 0x94d049bb133111ebULL;
        seed ^= (seed >> 31);
        std::ostringstream oss;
        oss << std::hex << std::setw(8) << std::setfill('0')
            << static_cast<uint32_t>(seed & 0xFFFFFFFF);
        return oss.str();
    }

    // Custom hash: mixes bits through multiple rounds, outputs 16-char hex
    static std::string computeHash(const std::string& input) {
        // Two independent hash chains for more bit coverage
        uint64_t h1 = 14695981039346656037ULL; // FNV offset basis
        uint64_t h2 = 0xcbf29ce484222325ULL;
        const uint64_t prime1 = 1099511628211ULL;
        const uint64_t prime2 = 1000000007ULL;

        for (size_t i = 0; i < input.size(); ++i) {
            uint8_t c = static_cast<uint8_t>(input[i]);
            // FNV-1a style mixing on h1
            h1 ^= c;
            h1 *= prime1;
            // Murmur-style mixing on h2
            h2 += c * (i + 1);
            h2 ^= (h2 >> 33);
            h2 *= prime2;
            h2 ^= (h2 >> 29);
            // Cross-mix every 4 bytes for avalanche
            if (i % 4 == 3) {
                h1 ^= (h2 >> 17);
                h2 ^= (h1 << 11);
            }
        }
        // Final mixing rounds
        h1 ^= h2;
        h1 ^= (h1 >> 23);
        h1 *= 0xff51afd7ed558ccdULL;
        h1 ^= (h1 >> 33);
        h2 ^= h1;
        h2 *= 0xc4ceb9fe1a85ec53ULL;
        h2 ^= (h2 >> 31);

        uint64_t finalHash = h1 ^ h2;
        std::ostringstream oss;
        oss << std::hex << std::setw(16) << std::setfill('0') << finalHash;
        return oss.str();
    }
};
