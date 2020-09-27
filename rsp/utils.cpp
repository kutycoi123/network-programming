#include "utils.hpp"

namespace utils {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string generateUuid() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }

    char* intToBytes(int number) 
    {
        char* bytes = new char[4];
        for(int i=0; i < 4; i++) {
            bytes[3-i] = (number >> (i*8));
        }
        return bytes;
    }

    int bytesToInt(const char* bytes) {
        return int((unsigned char)(bytes[0]) << 24 |
                   (unsigned char)(bytes[1]) << 16 |
                   (unsigned char)(bytes[2]) << 8 |
                   (unsigned char)(bytes[3]));
    }
}