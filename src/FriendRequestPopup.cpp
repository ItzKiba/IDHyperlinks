#include <Geode/Geode.hpp>
#include "utils.hpp"
#include "LevelPopup.hpp"
#include <Geode/modify/FriendRequestPopup.hpp>

using namespace geode::prelude;

class $modify(mFriendRequestPopup, FriendRequestPopup) {
    bool init(GJFriendRequest* p0) {
        if (!FriendRequestPopup::init(p0)) return false;
        std::string str = p0->m_message.c_str();
        std::vector<int> idlist = Utils::getIDList(str);
        if (idlist.size() == 0) return true;
        
        auto bgSpr = this->m_mainLayer->getChildByIDRecursive("description-background");
        if (!bgSpr) return true;

        auto btnSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        auto spr = CCMenuItemExt::createSpriteExtra(btnSpr, [idlist](auto){
            LevelPopup::create(idlist)->show();
        });
        auto menu = CCMenu::create();
        menu->setContentSize({0, 0});
        menu->setScale(0.35);
        menu->addChild(spr);
        menu->setPosition({15, 15});
        bgSpr->addChild(menu);

        handleTouchPriority(this);

        return true;
    }
};