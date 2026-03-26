#include "LevelPopup.hpp"

bool LevelPopup::init(std::vector<int> idlist) {
    if (!Popup::init({340.f, 190.f})) return false;

    int count = idlist.size();
    this->setTitle(fmt::format("{} level ID{} found", count, count == 1 ? "" : "s"));

    std::stringstream stream;
	bool first = true;
	for (unsigned int i = 0; i < idlist.size(); i++) {
		if (!first) {
			stream << ", ";
		}
		stream << std::to_string(idlist.at(i));
		first = false;
	}
    std::string label1 = fmt::format("Would you like to open the following level{}?", count == 1 ? "" : "s");
    std::string idlabel = stream.str();

    auto textarea1 = SimpleTextArea::create(label1, "chatFont.fnt", 1.0f);
    textarea1->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    textarea1->setZOrder(10);
    textarea1->setPosition({m_title->getPositionX(), m_title->getPositionY() - 20.f});
    textarea1->setScale(0.85f);
    auto textarea2 = SimpleTextArea::create(idlabel, "chatFont.fnt", 1.0f);
    textarea2->setAlignment(CCTextAlignment::kCCTextAlignmentCenter);
    textarea2->setWrappingMode(geode::WrappingMode::SPACE_WRAP);
    textarea2->setWidth(270);
    textarea2->setColor({0, 255, 128, 255});
    textarea2->setZOrder(10);
    
    auto scale9 = NineSlice::create("square02_001.png");
    scale9->setScaledContentSize({290, 80});
    scale9->setOpacity(75);

    m_mainLayer->addChild(textarea1);
    m_mainLayer->addChildAtPosition(scale9, Anchor::Center, {0, -4});
    m_mainLayer->addChildAtPosition(textarea2, Anchor::Center, {0, -4});

    auto menu = CCMenu::create();

    #ifdef GEODE_IS_WINDOWS
    auto copyBtnSpr = ButtonSprite::create("Copy");
    auto copyBtn = CCMenuItemExt::createSpriteExtra(copyBtnSpr, [idlabel, count](auto){
        // google
		if (OpenClipboard(nullptr)) {
            EmptyClipboard();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, idlabel.size() + 1);
            if (hMem) {
                memcpy(GlobalLock(hMem), idlabel.c_str(), idlabel.size() + 1);
                GlobalUnlock(hMem);
                SetClipboardData(CF_TEXT, hMem);
            }
            CloseClipboard();

            Notification::create(fmt::format("Copied ID{} to clipboard", count == 1 ? "" : "s"), CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
        }
	});
    menu->addChild(copyBtn);
    #endif

    auto openBtnSpr = ButtonSprite::create("Search");
    auto openBtn = CCMenuItemExt::createSpriteExtra(openBtnSpr, [idlist](auto){
		auto searchObj = Utils::getSearchObj(idlist);
		auto scene = LevelBrowserLayer::scene(searchObj);
		CCDirector::sharedDirector()->pushScene(scene);
	});
	menu->setContentSize({300, 0});
    menu->setLayout(
        RowLayout::create()
            ->setGap(40.f)
            ->setAxisAlignment(AxisAlignment::Center)
    );
    menu->addChild(openBtn);
    menu->updateLayout();
    m_mainLayer->addChildAtPosition(menu, Anchor::Bottom, {0, 25});

    return true;
}

LevelPopup* LevelPopup::create(std::vector<int> idlist) {
    auto ret = new LevelPopup();
    if (ret->init(idlist)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}