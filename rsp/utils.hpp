#include <random>
#include <sstream>

namespace utils {
    std::string generateUuid();

    char* intToBytes(int number);

    int bytesToInt(const char* bytes);
}