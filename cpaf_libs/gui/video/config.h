#pragma once

#include <functional>
#include <vector>
#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::gui::video {

class config
{
public:
    using changed_cb = std::function<void()>;

    config();

    const nlohmann::json& json() const { return jo_;  }
    const nlohmann::json& controls() const { return jo_["controls"];  }

    void    connect_for_changes (changed_cb callback);

private:
    void    signal_changed() const;

    nlohmann::json jo_;
    std::vector<changed_cb> on_changed_callbacks_;

};

} // namespace cpaf::gui::video

