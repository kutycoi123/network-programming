#include "set.hpp"


model::SetManager::SetManager() 
{ 
    _handler_map.insert(std::make_pair(OPT_CRT_SET, &model::SetManager::CreateSet));
    _handler_map.insert(std::make_pair(OPT_ADD_ITEM, &model::SetManager::AddItem));
    _handler_map.insert(std::make_pair(OPT_REM_ITEM, &model::SetManager::RemoveItem));
    _handler_map.insert(std::make_pair(OPT_GET_ITEMS, &model::SetManager::GetItems));
    _handler_map.insert(std::make_pair(OPT_CLR_SET, &model::SetManager::ClearSet));
    _handler_map.insert(std::make_pair(OPT_REM_SET, &model::SetManager::RemoveSet));
    _handler_map.insert(std::make_pair(OPT_GET_SIZE, &model::SetManager::GetSize));
    _handler_map.insert(std::make_pair(OPT_GET_SETS, &model::SetManager::GetSetIDs));
}

model::SetManager::SetManager(const model::SetManager& orig) 
{

}

model::SetManager::~SetManager()
{

}

model::Response* 
model::SetManager::HandleRequest(model::Request* req)
{
    const char opt = req->getOpt();
    std::map<char, RequestHandler>::iterator it = _handler_map.find(opt);
    if(it != _handler_map.end()) {
        return (*this.*it->second)(req);
    }
    return nullptr;
}
    
model::Response* 
model::SetManager::CreateSet(model::Request* req)
{
    std::string uuid = utils::generateUuid();
    Set* new_set = new Set();
    _set_map.insert(std::make_pair(uuid, new_set));
    
    model::Response* res = new model::Response(req->getOpt(), RET_SUCCESS);
    res->addUuid(uuid);
    
    std::cout << "[SERVER] New set created:" << uuid << std::endl;
    return res;
}

model::Response* 
model::SetManager::AddItem(model::Request* req)
{
    model::Response* res;
    std::string req_uuid = req->getUuid();
    SetMap::iterator it = _set_map.find(req_uuid);
    if(it != _set_map.end()) {
        std::cout << "[SERVER] Set is found" << std::endl;
        int item = req->getItem();
        Set* s_set = it->second;
        Set::iterator s_it = s_set->find(item);
        if(s_it == s_set->end()) 
        {
            std::cout << "[SERVER] Adding item:" << item << std::endl;
            it->second->insert(req->getItem());
            res = new model::Response(req->getOpt(), RET_SUCCESS);
        } else {
            std::cout << "[SERVER] Item " << item << " exists" << std::endl;
            res = new model::Response(req->getOpt(), RET_ITEM_ALREADY_EXIST);
        }        
    } else {
        std::cout << "[SERVER] Set is *not* found" << std::endl;
        res = new model::Response(req->getOpt(), RET_SET_NOT_EXIST);
    }

    return res;
}

model::Response* 
model::SetManager::RemoveItem(model::Request* req)
{
    model::Response* res;
    std::string req_uuid = req->getUuid();
    SetMap::iterator it = _set_map.find(req_uuid);
    if(it != _set_map.end()) {
        std::cout << "[SERVER] Set is found" << std::endl;
        int item = req->getItem();
        Set* s_set = it->second;
        Set::iterator s_it = s_set->find(item);
        if(s_it != s_set->end()) 
        {
            std::cout << "[SERVER] Removing item:" << item << std::endl;
            it->second->erase(item);
            res = new model::Response(req->getOpt(), RET_SUCCESS);
        } else {
            std::cout << "[SERVER] Item:" << item << " is *not* found" << std::endl;
            res = new model::Response(req->getOpt(), RET_ITEM_NOT_EXIST);
        }
    } else {
        std::cout << "[SERVER] Set is *not* found" << std::endl;
        res = new model::Response(req->getOpt(), RET_SET_NOT_EXIST);
    }

    return res;
}

model::Response* 
model::SetManager::GetItems(model::Request* req)
{
    model::Response* res;
    std::string req_uuid = req->getUuid();
    SetMap::iterator it = _set_map.find(req_uuid);
    if(it != _set_map.end()) {
        std::cout << "[SERVER] Set is found" << std::endl;
        std::cout << "[SERVER] Retrieving items..." << std::endl;
        Set* s_set = it->second;

        res = new model::Response(req->getOpt(), RET_SUCCESS);
        for(auto item : *s_set) {
            res->addItem(item);
        }
    } else {
        std::cout << "[SERVER] Set is *not* found" << std::endl;
        res = new model::Response(req->getOpt(), RET_SET_NOT_EXIST);
    }

    return res;
}

model::Response* 
model::SetManager::ClearSet(model::Request* req)
{
    model::Response* res;
    std::string req_uuid = req->getUuid();
    SetMap::iterator it = _set_map.find(req_uuid);
    if(it != _set_map.end()) {
        std::cout << "[SERVER] Set is found" << std::endl;
        std::cout << "[SERVER] Clearing items..." << std::endl;
        Set* s_set = it->second;
        s_set->clear();
        res = new model::Response(req->getOpt(), RET_SUCCESS);
    } else {
        std::cout << "[SERVER] Set is *not* found" << std::endl;
        res = new model::Response(req->getOpt(), RET_SET_NOT_EXIST);
    }

    return res;
}

model::Response* 
model::SetManager::RemoveSet(model::Request* req)
{
    model::Response* res;
    std::string req_uuid = req->getUuid();
    SetMap::iterator it = _set_map.find(req_uuid);
    if(it != _set_map.end()) {
        std::cout << "[SERVER] Set is found" << std::endl;
        std::cout << "[SERVER] Removing a set..." << std::endl;
        _set_map.erase(it);
        res = new model::Response(req->getOpt(), RET_SUCCESS);
    } else {
        std::cout << "[SERVER] Set is *not* found" << std::endl;
        res = new model::Response(req->getOpt(), RET_SET_NOT_EXIST);
    }

    return res;
}

model::Response* 
model::SetManager::GetSize(model::Request* req)
{
    model::Response* res;
    std::string req_uuid = req->getUuid();
    SetMap::iterator it = _set_map.find(req_uuid);
    if(it != _set_map.end()) {
        std::cout << "[SERVER] Set is found" << std::endl;
        Set* s_set = it->second;

        res = new model::Response(req->getOpt(), RET_SUCCESS);
        res->setSetSize(s_set->size());
    } else {
        std::cout << "[SERVER] Set is *not* found" << std::endl;
        res = new model::Response(req->getOpt(), RET_SET_NOT_EXIST);
    }

    return res;
}

model::Response* 
model::SetManager::GetSetIDs(model::Request* req)
{
    model::Response* res;
    res = new model::Response(req->getOpt(), RET_SUCCESS);
    for(SetMap::iterator it=_set_map.begin(); it != _set_map.end(); ++it)
    {
        res->addUuid(it->first);
    }
    return res;
}