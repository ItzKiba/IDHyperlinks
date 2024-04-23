#include <Geode/Geode.hpp>
#include <vector>
#include <string>
using namespace geode::prelude;

void fillIDList(std::vector<int> &v, std::string str, std::string &newStr) {
	std::string match = "";
	str = str + " ";
	int matchCount = 0;

	std::vector<int> closeTags;
	std::vector<int> openTags;

	for (int i = 0; i < str.length(); i++) {
		if (std::isdigit(str.at(i))) {
			match.push_back(str.at(i));
			matchCount++;
		} else {
			if (matchCount >= 8) {
				int append = std::stoi(match);
				v.push_back(append);

				closeTags.push_back(i);
				openTags.push_back(i - matchCount);
			}
			matchCount = 0;
			match = "";
		}
	}

	newStr = str;
	for (int i = closeTags.size() - 1; i >= 0; i--) {
		newStr.insert(closeTags.at(i), "</c>");
		newStr.insert(openTags.at(i), "<cg>");
	}
}

GJSearchObject* getSearchObj(std::vector<int> v) {
	std::stringstream download;
	bool first = true;
	for (unsigned int i = 0; i < v.size(); i++) {
		if (!first) {
			download << ",";
		}
		download << std::to_string(v.at(i));
		first = false;
	}
	
	download << "&gameVersion=22";
	GJSearchObject* searchObj = GJSearchObject::create(SearchType::Type19, download.str());
	return searchObj;
}

/*
Pass in more than 1 parameter to the TextArea button.

There were originally more parameters so that's why I'm
using a struct for this...
*/
struct BtnParameters : public CCObject {
    std::vector<int> m_vector;
    BtnParameters(std::vector<int> v) : m_vector(v) {
        this->autorelease();
    }
};

float calculateTextAreaXOffset(TextArea* textArea) {
	int largestChildCount = 0;
	CCObject* sobj1;
	CCARRAY_FOREACH(textArea->getChildren(), sobj1) {
		auto mbf = typeinfo_cast<MultilineBitmapFont*>(sobj1);
		if (mbf != nullptr) {
			CCObject* sobj2;
			CCARRAY_FOREACH(mbf->getChildren(), sobj2) {
				auto bmlf = typeinfo_cast<CCLabelBMFont*>(sobj2);
				if (bmlf != nullptr) {
					if (bmlf->getChildrenCount() > largestChildCount) {
						largestChildCount = bmlf->getChildrenCount();
					}
				}
			}
			break;
		}
	}

	float length = largestChildCount * 5.f;

	float divisor = 1.2 + (4 * (1 - (370 - length) / 370));
	float offset = (370 - length) / divisor;
	return offset;
}

#include <Geode/modify/TextArea.hpp>
class $modify(MyTextArea, TextArea) {
	std::vector<int> m_idList;

	bool init(gd::string p0, char const* p1, float p2, float p3, cocos2d::CCPoint p4, float p5, bool p6) {
		this->retain();
		Loader::get()->queueInMainThread([this, p0, p1, p2, p3, p4, p5, p6] {
			
			CCLayer* parentLayer = typeinfo_cast<CCLayer*>(this->getParent());
			if (parentLayer == nullptr) {
				return;
			}
			GJMessagePopup* popup = typeinfo_cast<GJMessagePopup*>(parentLayer->getParent());
			if (popup == nullptr) {
				return;
			}

			this->m_disableColor = false;
			std::vector<int> idList = this->m_fields->m_idList;
			std::string newMessageString;

			fillIDList(idList, p0, newMessageString);
			if (idList.size() == 0) {
				return;
			}
			this->setString(newMessageString);
			
			auto pos = parentLayer->getContentSize() / 2;
			auto menu = CCMenu::create();
			menu->setPosition(pos);
			parentLayer->addChild(menu);

			auto btnSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn01_001.png");
			btnSprite->setContentSize(btnSprite->getContentSize() * 4.f);

			auto btn = CCMenuItemSpriteExtra::create(btnSprite, this, menu_selector(MyTextArea::onBtn));
			btn->setUserObject(new BtnParameters(idList));
			btn->setOpacity(0);
			
			float offset = calculateTextAreaXOffset(this);
			this->setPositionY(60.f);
			this->setPositionX(offset);

			menu->setAnchorPoint({0.5, 0.5});
			menu->addChild(btn);

			this->removeFromParent();
			btn->addChild(this);

			handleTouchPriority(popup);

			this->release();
		});
	
		if (!TextArea::init(p0, p1, p2, p3, p4, p5, p6)) {
			return false;
		}

		return true;
	}

	void onBtn(CCObject* sender) {
		auto params = static_cast<BtnParameters*>(static_cast<CCNode*>(sender)->getUserObject());
		auto searchObj = getSearchObj(params->m_vector);
		auto scene = LevelBrowserLayer::scene(searchObj);
		CCDirector::sharedDirector()->pushScene(scene);
	}
};

#include <Geode/modify/GJMessagePopup.hpp>
class $modify(MyMessagePopup, GJMessagePopup) {
	std::vector<int> m_idList;
	void loadFromGJMessage(GJUserMessage* p0) {
		GJMessagePopup::loadFromGJMessage(p0);

		// find text label
		CCLabelBMFont* label = nullptr;
		CCObject* obj;
		auto parentLayer = this->m_mainLayer;
		CCARRAY_FOREACH(parentLayer->getChildren(), obj) {
			auto test = typeinfo_cast<CCLabelBMFont*>(obj);
			if (test != nullptr && test->getPositionY() == 264) {
				label = test;
				break;
			}
		}

		if (label == nullptr) {
			return;
		}

		std::vector<int> idList = this->m_fields->m_idList;
		std::string newMessageString;

		fillIDList(idList, label->getString(), newMessageString);
		if (idList.size() == 0) {
			return;
		}

		auto pos = label->getPosition();
		auto menu = CCMenu::create();
		menu->setPosition(pos);
		parentLayer->addChild(menu);

		auto btnSprite = CCLabelBMFont::create(label->getString(), "bigFont.fnt");
		btnSprite->setColor({0, 255, 0});
		btnSprite->setScale(0.6);

		auto btn = CCMenuItemSpriteExtra::create(btnSprite, this, menu_selector(MyTextArea::onBtn));
		btn->setUserObject(new BtnParameters(idList));
		label->setVisible(false);

		menu->setAnchorPoint({0.5, 0.5});
		menu->addChild(btn);

		handleTouchPriority(this);

	}
};

/*
The entire purpose of this hook is to fix a bug by resetting the touch priority
of the GJMessagePopup layer
*/
#include <Geode/modify/CCLayer.hpp>
class $modify(MyLayer, CCLayer) {
	void onEnter() {
		CCLayer::onEnter();
		GJMessagePopup* popup = typeinfo_cast<GJMessagePopup*>(this);
		if (popup == nullptr) {
			return;
		}
	
		handleTouchPriority(popup);

	}
};


