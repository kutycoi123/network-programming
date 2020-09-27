#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <cstring>
#include <iostream>

#include "common.hpp"
#include "utils.hpp"

namespace model {

    class Request
    {
    public:
        Request(char opt);
        Request(char opt, std::string uuid);
        Request(char opt, std::string uuid, int item);
        Request(const char* msg);
        Request(const Request& orig);
        virtual ~Request();

        char* toBytes();
        int getBytesCount();

        const char getOpt();
        std::string getUuid();
        int getItem();
        bool hasUuid();
        bool hasItem();


    private:
        char _opt;
        std::string _uuid;
        int _item;
        bool _has_uuid;
        bool _has_item;

        void setup(char opt, std::string uuid, int item);
        void setupFlags(char opt);
    };
}
#endif /* REQUEST_HPP */
