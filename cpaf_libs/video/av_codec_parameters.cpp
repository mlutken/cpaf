
#include "av_codec_parameters.h"

#include <utility>
#include <ios>

using namespace std;
namespace cpaf::video {

// -------------------------
// --- Constructors etc. ---
// -------------------------

av_codec_parameters::av_codec_parameters(AVCodecParameters* ff_codec_parameters)
	: ff_codec_parameters_(ff_codec_parameters)
{
}

av_codec_parameters::~av_codec_parameters()
{
	if (ff_codec_parameters_) {
		// TODO: Do we need to do anything here? xxx_close(&ff_codec_parameters_);
	}
}

// -----------------------
// --- Debug functions ---
// -----------------------

void av_codec_parameters::dump() const
{
}

} //END namespace cpaf::video
