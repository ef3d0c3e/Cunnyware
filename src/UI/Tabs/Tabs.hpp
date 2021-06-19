#ifndef UI_TABS_HPP
#define UI_TABS_HPP

#include "../Tab.hpp"

typedef std::vector<std::vector<void (*)()>> TabContent;

namespace Tabs
{
	void Visuals();
	void Configs();
}

#endif // UI_TABS_HPP
