#ifndef SET_HPP
#define SET_HPP

#include <iostream>
#include <map>
#include <set>
#include <string>

#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"


typedef std::set<int> Set;
typedef std::map<std::string, Set*> SetMap;

namespace model {
    class SetManager {
        typedef model::Response* (SetManager::*RequestHandler) (model::Request* req);
        public:
        SetManager();
        SetManager(const SetManager& orig);
        virtual ~SetManager();

        model::Response* HandleRequest(Request* req);
    
        private:
        SetMap _set_map;
        std::map<char, RequestHandler> _handler_map;

        Response* CreateSet(Request* req);
        Response* AddItem(Request* req);
        Response* RemoveItem(Request* req);
        Response* GetItems(Request* req);
        Response* ClearSet(Request* req);
        Response* RemoveSet(Request* req);
        Response* GetSize(Request* req);
        Response* GetSetIDs(Request* req);
    };
}

#endif