
#include <iostream>

#include <base/v2.hpp>
#include <primitives2d/line_adapter.hpp>
#include <detect_utils.h>
#include <CircleDetect.h>
#include <Color.h>
#include <color_edge_detect_policies.h>
#include <color_pixel_get_policies.h>

using namespace std;
using namespace detect;
using namespace gmath;
using namespace gmath::p2d;

int main(int argc, char* argv[])
{
    typedef line_adapter<float, GetDummyColorPixelPolicy>  Line;
    typedef CircleDetect<Color, GetDummyColorPixelPolicy, ColorEdgeDefaultPolicy>  CircleDetector;


    cout << " Hello from CPAF/CBS ! "<<endl;
    circle2f c1 = circle2f( v2f(2,2), 21 );
    CircleDetector cd( c1 );

    cout << "CircleDetector: " << cd << endl;
    cout << "getPixel: " << cd.getPixel(0,0) << endl;

    cd.detect();

    Line l(v2f(14,17), v2f(8,5));

    cout << "vec_step: " << l.vec_step() << endl;
    cout << "len: " << l.len() << endl;
    cout << "size: " << l.size() << endl;

    for (size_t i = 0; i < l.size(); ++i) {
        cout << "["  << i << "]: " << l.index_to_pos(i) << " pixel pos: " << l.index_to_pixel_pos(i) << endl;
    }



//    detect_utils_fun1();
//    detect_utils_fun2();
    return 0;
}

