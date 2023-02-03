
#include <iostream>

#include <QApplication>
#include <QPushButton>

#include <base/v2.hpp>
#include <primitives2d/line_adapter.hpp>
#include <detect_utils.h>
#include <CircleDetect.h>
#include <Color.h>
#include <color_edge_detect_policies.h>
#include <color_pixel_get_policies.h>

#include <DetectWidget.h>

using namespace std;
using namespace detect;
using namespace gmath;
using namespace gmath::p2d;

int main(int argc, char **argv)
{
    QApplication app (argc, argv);

//    QPushButton button ("Hello world !");
//    button.show();

    DetectWidget dw;
    dw.loadImage("./images/blue_eye_normal_open_centered.jpg");
    dw.show();

    return app.exec();
}
