#include <iostream>
#include <array>
#include <fmt/format.h>

#include <imgui.h>

#include <cpaf_libs/gui/system.h>
#include <cpaf_libs/gui/app.h>


using namespace std;
//using namespace std::chrono_literals;
using namespace cpaf;
using namespace cpaf::gui;


class myapp : public cpaf::gui::app
{
public:
    using app::app;
protected:
    void                            frame_update                () override;
    events::is_handled              event_handler               (const events::event& evt) override;

private:
    bool                      m_show_some_panel {true};
    bool                      m_show_debug_panel{false};
};


int main(int /*argc*/, char* /*argv*/ [])
{
    cpaf::gui::system::instance().init();
    fmt::println("*** CPAF GUI PLAYGROUND 1 ***");
    fmt::println("sizeof(events::event) : {}", sizeof(events::event));
;
//    myapp app{"App"};
    myapp app;
    app.run();

	return 0;
}

void myapp::frame_update()
{
    ImGui::DockSpaceOverViewport();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Cmd+Q")) {
                stop();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Some Panel", nullptr, &m_show_some_panel);
            ImGui::MenuItem("Debug Panel", nullptr, &m_show_debug_panel);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    // Whatever GUI to implement here ...

    // ImGui::SetCursorPos({0,0});
    if (m_show_some_panel) {
        ImGui::Begin("Some panel", &m_show_some_panel);
        ImGui::Text("Hello World dsf");
        ImGui::End();
    }

    // Debug panel
    if (m_show_debug_panel) {
        ImGui::Begin("Debug panel", &m_show_debug_panel);
        ImGui::Text("User config path: %s", config_path().string().c_str());
        ImGui::Separator();
        // ImGui::Text("Font path: %s", font_path.c_str());
        // ImGui::Text("Font size: %f", font_size);
        // ImGui::Text("Global font scaling %f", io.FontGlobalScale);
        // ImGui::Text("UI scaling factor: %f", font_scaling_factor);
        ImGui::End();
    }


}

events::is_handled myapp::event_handler(const events::event& evt)
{
    // fmt::println("GUI 1 Playground myapp::event_handler(): {} ", evt.to_string(events::to_str_mode::normal));
    // std::cout << std::flush;
    return app::event_handler(evt);
}
