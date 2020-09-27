#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <cstring>
#include <vector>
#include <iostream>

#include "common.hpp"
#include "utils.hpp"

namespace model {

    class Response
    {
    public:
        Response(char opt, char ret);
        Response(char opt, const char* msg);
        Response(const Response& orig);
        virtual ~Response();

        char* toBytes();
        int getBytesCount();

        const char getOpt();
        const char getRet();
        int getLen();

        void addItem(int item);
        void addUuid(std::string uuid);
        void setSetSize(int size);
        
        const std::vector<int> getItems();
        const std::vector<std::string>getUuids();
        int getSetSize();

    private:
        char _opt;
        char _ret;
        int _len;
        std::vector<int> _items;
        std::vector<std::string> _uuids;
        int _set_size;
    };
}
#endif /* RESPONSE_HPP */
