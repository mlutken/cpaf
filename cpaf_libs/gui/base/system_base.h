#pragma once
#include <cstdint>


namespace cpaf::gui {


class system_base {
public:
	system_base() = default;
	virtual ~system_base() = default;

    int32_t					init						() { return do_init();  }
    int32_t                 display_count               () const { return do_display_count();   }
    float                   default_dpi                 () const { return do_dpi(0);   }
    float                   dpi                         (int32_t display_index) const { return do_dpi(display_index);   }

	// --- Debug functions ---

protected:

private:
    virtual int32_t			do_init						() = 0;
    virtual int32_t			do_display_count			() const = 0;
    virtual float			do_dpi						(int32_t display_index) const = 0;

};


} //END namespace cpaf::gui




