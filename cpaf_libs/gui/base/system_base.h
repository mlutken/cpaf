#pragma once
#include <cstdint>


namespace cpaf::gui {


class system_base {
public:
	system_base() = default;
	virtual ~system_base() = default;

	int32_t					init						() { return do_init(); }

	// --- Debug functions ---

protected:

private:
	virtual int32_t			do_init						() = 0;

};


} //END namespace cpaf::gui




