#include <iostream>
#include <sstream>
#include <string>
#include <deque>
#include <algorithm>
#include <stdexcept>


std::string createIp() {
    std::string result;
    for (int i = 0; i < 4; i++) {
        if (!result.empty()) {
            result += ".";
        }

        uint8_t byte = static_cast<uint8_t>(rand() % 256);
        result += std::to_string(byte);
    }
    
    return result;
}

uint32_t parseLine(const std::string& line) {
    auto start = 0;
    uint8_t ip[4];
    int index = 0;
    size_t pos = 0;

    while (pos != std::string::npos) {
        pos = line.find('.', start);
        ip[index] = static_cast<uint8_t>(std::stoi(line.substr(start, pos - start)));
        start = pos + 1;
        index++;
    }

    if (index != 4) {
        throw std::logic_error("Error in line: " + line);
    }

    return (ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8) | ip[3];
}


std::string parseUint32Ip(uint32_t ip) {
    std::string result;
    for (int i = 0; i < 4; ++i) {
        if (!result.empty()) {
            result += ".";
        }
        uint8_t byte = (ip >> (8 * (3 - i))) & 0xFF;
        result += std::to_string(static_cast<int>(byte));
    }
    return result;
}

bool reverseLexicographicalCompare(uint32_t a, uint32_t b) {
    return a > b;
}

std::ostream& operator<<(std::ostream& out, std::deque<uint32_t> result) {
    for (auto ip : result) {
        out << parseUint32Ip(ip) << std::endl;
    }
    return out;
}

struct TypesOfIp {
    static inline std::deque<uint32_t> all_ips{};
    static inline std::deque<uint32_t> first_byte_1{};
    static inline std::deque<uint32_t> first46_second70{};
    static inline std::deque<uint32_t> any_46{};
};

int test() {
    std::string line;
    std::deque<uint32_t> adresses;
    while(std::getline(std::cin, line)) {
        if (line == "c") {
            std::cout << adresses << std::endl;
            return 1;
        }
        auto rand_ip = createIp();
        std::cout << "Created ip: " << rand_ip << "\n";
        auto ip32 = parseLine(rand_ip);
        std::cout << "Parsed ip: " << ip32 << "\n";
        auto it = std::upper_bound(adresses.begin(), adresses.end(), ip32, reverseLexicographicalCompare);
        adresses.insert(it, ip32);
    }
    return 0;
}

template<typename Container>
void fillAdresses(Container& adresses, uint32_t ip32) {
    auto it = std::upper_bound(adresses.begin(), adresses.end(), ip32, reverseLexicographicalCompare);
    adresses.insert(it, ip32);
}

void fillTypeOfIps(uint32_t ip32) {
    fillAdresses(TypesOfIp::all_ips, ip32);

    auto byte1 = static_cast<uint8_t>(ip32 >> 24);
    auto byte2 = static_cast<uint8_t>(ip32 >> 16);

    if (byte1 == 1) {
        fillAdresses(TypesOfIp::first_byte_1, ip32);
    }

    if (byte1 == 46 && byte2 == 70) {
        fillAdresses(TypesOfIp::first46_second70, ip32);
    }

    bool has46 = false;

    for (int i = 0; i < 4; i++) {
        uint8_t byte = (ip32 >> (8 * (3 - i))) & 0xFF;
        if (byte == 46) {
            has46 = true;
            break;
        }
    }

    if (has46) {
        fillAdresses(TypesOfIp::any_46, ip32);
    }
    
}

void printResults() {
    std::cout << TypesOfIp::all_ips;
    std::cout << TypesOfIp::first_byte_1;
    std::cout << TypesOfIp::first46_second70;
    std::cout << TypesOfIp::any_46;
}



int main() {
    std::string line;

    while (std::getline(std::cin, line)) {
        size_t tab_pos = line.find('\t');
        std::string ip_str = (tab_pos != std::string::npos) ? line.substr(0, tab_pos) : line;

        try {
            fillTypeOfIps(parseLine(ip_str));
        } catch (...) {
            continue;
        }
    }

    printResults();

    return 0;
}