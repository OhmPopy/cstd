#ifndef NANA_DETAIL_BEDROCK_PI_DATA_HPP
#define NANA_DETAIL_BEDROCK_PI_DATA_HPP



#include <nana/gui/detail/bedrock.hpp>
#include "color_schemes.hpp"
#include "events_operation.hpp"
#include "window_manager.hpp"
#include <set>

namespace nana
{
	namespace detail
	{
		struct bedrock::pi_data
		{
			color_schemes				scheme;
			events_operation			evt_operation;
			window_manager				wd_manager;
			set<core_window_t*>	auto_form_set;
			bool shortkey_occurred{ false };

			struct menu_rep
			{
				core_window_t*	taken_window{ NULL };
				bool			delay_restore{ false };
				native_window_type window{ NULL };
				native_window_type owner{ NULL };
				bool	has_keyboard{ false };
			}menu;
		};
	}
}



#endif
