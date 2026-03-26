#include <Geode/Geode.hpp>
#include "utils.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCLayer.hpp>
class $modify(MyLayer, CCLayer) {
	void onEnter() {
		CCLayer::onEnter();

        FriendRequestPopup* fpopup = typeinfo_cast<FriendRequestPopup*>(this);
        if (fpopup != nullptr) {
            handleTouchPriority(fpopup);
        }

		GJMessagePopup* popup = typeinfo_cast<GJMessagePopup*>(this);
		if (popup != nullptr) {
			handleTouchPriority(popup);
		}

	}
};