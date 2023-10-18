#include "app_base.h"

#include <sstream>
#include <cpaf_libs/gui/system_window.h>

namespace cpaf::gui {

exit_status_t app_base::run()
{
    do_run();
    return exit_status_;
}




// -------------------------------------------------------
// --- PROTECTED: Helper functions for derived classes ---
// -------------------------------------------------------




} // END namespace cpaf::gui

