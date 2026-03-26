#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class Utils {
public:
    static GJSearchObject* getSearchObj(std::vector<int> vec);
    static std::vector<int> getIDList(std::string str);
};