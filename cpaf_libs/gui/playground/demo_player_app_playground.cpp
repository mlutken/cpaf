#include <iostream>
#include <array>
#include <boost/program_options.hpp>
#include <fmt/format.h>


#include <cpaf_libs/gui/video/data_definitions/playable.h>
#include <cpaf_libs/gui/video/data_definitions/playlist.h>
#include <cpaf_libs/gui/video/demo_player_app.h>


using namespace std;
using namespace cpaf;
using namespace cpaf::gui;
using namespace cpaf::gui::video;
namespace po = boost::program_options;


nlohmann::json parse_command_line(int argc, const char *argv[]);


// ------------
// --- Main ---
// ------------
int main(int argc, const char *argv[])
{
    std::cerr << "*** CPAF Demo Video player ***\n";

    cpaf::gui::system::instance().init();
    cpaf::audio::system::instance().init();

    const auto jo = parse_command_line(argc, argv);
    demo_player_app app;
    //app.configuration().controls_set("skip_time_small", 1);
    // app.configuration().controls_set("relative_ypos", 0.7);


    app.window_title_set("CPAF demo video player");
    app.set_initial_playlist(jo["playlist"]);
    app.set_initial_start_time(0min);

    app.run();

    return 0;
}

// ------------------------
// --- Helper functions ---
// ------------------------
nlohmann::json parse_command_line(int argc, const char *argv[])
{

    auto playlist = cpaf::gui::video::playlist::create_json();


    auto primary_playable = cpaf::gui::video::playable();

    po::variables_map vm;
    using namespace std;

    std::string file_data;

    try {
        // Declare a group of options that will be allowed only on command line
        po::options_description basic("Info");
        basic.add_options()
            ("version,v",   "print version string")
            ("help,h",      "produce help message")
            ;

               // Declare a group of options that will be
               // allowed both on command line and in config file
        po::options_description config("Run");
        config.add_options()
            ("file,f", po::value<std::string>(), "File/URL to play")
            ("subtitle,s", po::value<std::string>(), "[optional] Subtitle file to use")
            ("subtitle-lang,sl", po::value<std::string>(), "[optional] Subtitle language code")
            ;

               // Hidden options, will be allowed both on command line and
               // in config file, but will not be shown to the user.
               // Hidden options are those without argument names (unanmed arguments).
               // In this case we use them for the files to play
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("files", po::value< std::vector<std::string> >(), "Files/URLs to play")
            ;


        po::options_description cmdline_options;
        cmdline_options.add(basic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(basic).add(config);

        po::positional_options_description p;
        p.add("files", -1);

        po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        po::notify(vm);

        notify(vm);

        if (vm.count("help")) {
            std::cout << visible << "\n";
            exit(0);
        }

        if (vm.count("version")) {
            fmt::println("Demo video Player version 1.0.0");
            exit(0);
        }

        const auto count_files = vm.count("files");

        if (vm.count("file")) {
            fmt::println("DBG: file {}", vm["file"].as<string>()); std::cout << std::endl;
            primary_playable.set_path(vm["file"].as<string>());
        }
        else if (vm.count("files") == 1) {
            const auto files = vm["files"].as<vector<string>>();
            primary_playable.set_path(files[0]);
        }

        if (vm.count("subtitle")) {
            primary_playable.set_subtitle_user(vm["subtitle"].as<string>());
            if (vm.count("subtitle-lang")) {
                primary_playable.add_subtitle(vm["subtitle"].as<string>(), vm["subtitle-lang"].as<string>());
            }
        }

        if (primary_playable.has_path()) {
            playlist["items"].push_back(primary_playable.json());
        }

               // Additional files to add to playlist
        if (vm.count("files") > 1) {
            const auto files = vm["files"].as<vector<string>>();
            cpaf::gui::video::playlist::add_playables(playlist, files, "");
        }
    }
    catch(std::exception& e)
    {
        std::cout << "Error: Unknown arguments given:" << std::endl;
        std::cout << e.what() << "\n";
        exit(1);
    }

    nlohmann::json jo;
    jo["config"] = nlohmann::json::object();
    jo["playlist"] = playlist;

    return jo;
}
