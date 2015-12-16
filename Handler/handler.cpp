//
// Created by root on 16.12.15.
//
//#include "handler.h"
//
//
//
//static void Handler::updateMap(std::string oldChallange, std::string username,std::string newChallange)
//{
//    std::map<std::string,std::string>::iterator it = m_usernameChallangeMap.find(oldChallange);
//    if (it != m_usernameChallangeMap.end())
//        m_usernameChallangeMap.erase(it);
//    m_usernameChallangeMap.insert(std::pair<std::string,std::string>(newChallange,username));
//}
//
//void Handler::insertToMap(std::string challange, std::string username)
//{
//    m_usernameChallangeMap.insert(std::pair<std::string,std::string>(challange,username));
//}
//
//static void Handler::removeFromMap(std::string challange)
//{
//    std::map<std::string,std::string>::iterator it = m_usernameChallangeMap.find(challange);
//    if (it != m_usernameChallangeMap.end())
//        m_usernameChallangeMap.erase(it);
//}
