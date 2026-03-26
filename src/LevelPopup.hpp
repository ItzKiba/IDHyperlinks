#pragma once

#include <Geode/Geode.hpp>
#include <vector>
#include "utils.hpp"
using namespace geode::prelude;

class LevelPopup : public geode::Popup {
protected:
    bool init(std::vector<int> idlist);
public:
    static LevelPopup* create(std::vector<int> idlist);
};