
#include "av_codec.h"

#include <utility>
#include <ios>

using namespace std;
namespace cpaf::video {

// -------------------------
// --- Constructors etc. ---
// -------------------------

av_codec::av_codec(AVCodec* ff_codec)
    : ff_codec_(ff_codec)
{
}

av_codec::~av_codec()
{
    if (ff_codec_) {
		// TODO: Do we need to do anything here? xxx_close(&codec_context_);
	}
}

// -----------------------
// --- Debug functions ---
// -----------------------

void av_codec::dump() const
{
}

} //END namespace cpaf::video
