#include <Geode/Geode.hpp>
#include "utils.hpp"
#include "LevelPopup.hpp"
#include <Geode/modify/GJMessagePopup.hpp>

using namespace geode::prelude;

class $modify(mGJMessagePopup, GJMessagePopup) {
	void loadFromGJMessage(GJUserMessage* p0) {
		GJMessagePopup::loadFromGJMessage(p0);

        std::string content = p0->m_content.c_str();
        std::string title = p0->m_title;
        std::string str = content + " " + title;
        std::vector<int> idlist = Utils::getIDList(str);
        if (idlist.size() == 0) return;
		
        // rahh there are no node ids for this shit

        CCScale9Sprite* bgSpr;
		CCObject* obj;
		auto parentLayer = this->m_mainLayer;
        for (auto obj : CCArrayExt(parentLayer->getChildren())) {
            auto test = typeinfo_cast<CCScale9Sprite*>(obj);
			if (test != nullptr && test->getOpacity() == 75) {
				bgSpr = test;
				break;
			}
        }

        if (!bgSpr) return;
        auto btnSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
        auto spr = CCMenuItemExt::createSpriteExtra(btnSpr, [idlist](auto){
            LevelPopup::create(idlist)->show();
        });
        auto menu = CCMenu::create();
        menu->setContentSize({0, 0});
        menu->setScale(0.35);
        menu->addChild(spr);
        menu->setTouchEnabled(true);
        menu->setPosition({15, 15});
        bgSpr->addChild(menu);

        handleTouchPriority(this);
    }
};
