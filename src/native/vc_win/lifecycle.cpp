#include "native/vc_win/console_color.hpp"
#ifdef BE_NATIVE_VC_WIN

namespace be {

///////////////////////////////////////////////////////////////////////////////
void native_init() {
	detail::init_console_color_handlers();
}

///////////////////////////////////////////////////////////////////////////////
void native_deinit() { }

} // be

#endif
