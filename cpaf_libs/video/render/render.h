#ifndef CPAF_VIDEO_RENDER_H
#define CPAF_VIDEO_RENDER_H

#include <config/cpaf_platform_definitions.h>

#if (CPAF_GRAPHICS_SYSTEM_ID == CPAF_SYSTEM_ID_SDL2)
#include <cpaf_libs/video/render/render__sdl2.h>
#endif

namespace cpaf::video {

class render : public render_platform
{
public:
    render();
};

} //END namespace cpaf::video

#endif // CPAF_VIDEO_RENDER_H
