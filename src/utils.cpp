#include "utils.hpp"

std::vector<int> Utils::getIDList(std::string str) {
	std::vector<int> new_vec = {};
    std::string match = "";
	int matchCount = 0;

	int digitsMin = Mod::get()->getSettingValue<int64_t>("min-digits");

	for (int i = 0; i < str.length(); i++) {
		if (std::isdigit(str.at(i))) {
			match.push_back(str.at(i));
			matchCount++;
		} else {
			if (matchCount >= digitsMin) {
                auto append = geode::utils::numFromString<int>(match);
				int appendInt = append.unwrapOr(0);
		        new_vec.push_back(appendInt);
			}
			matchCount = 0;
			match = "";
		}
	}

	if (matchCount >= digitsMin) {
		auto append = geode::utils::numFromString<int>(match);
        int appendInt = append.unwrapOr(0);
		new_vec.push_back(appendInt);
	}

	return new_vec;
}

GJSearchObject* Utils::getSearchObj(std::vector<int> vec) {
	std::stringstream download;
	bool first = true;
	for (unsigned int i = 0; i < vec.size(); i++) {
		if (!first) {
			download << ",";
		}
		download << std::to_string(vec.at(i));
		first = false;
	}
	
	download << "&gameVersion=22";
	GJSearchObject* searchObj = GJSearchObject::create(SearchType::Type19, download.str());
	return searchObj;
}