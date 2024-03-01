#pragma once

#include <functional>
#include <vector>
#include <chrono>
#include <cpaf_libs/utils/cpaf_json_utils.h>
#include <cpaf_libs/gui/color.h>

namespace cpaf::gui::video {

class config
{
public:
    using changed_cb = std::function<void()>;

    config();
    void                        set                 (const nlohmann::json& jo);

    const nlohmann::json&       json                () const { return jo_;  }
    const nlohmann::json&       controls            () const { return jo_["controls"];  }
    void                        controls_set        (const std::string& id, const nlohmann::json& val);
    cpaf::gui::color            controls_color      (const std::string& id) const;
    std::string                 controls_str        (const std::string& id) const;
    float                       controls_float      (const std::string& id) const;
    std::int32_t                controls_int32      (const std::string& id) const;
    std::chrono::seconds        controls_seconds    (const std::string& id) const;

    // template <class TYPE>
    // TYPE                    controls_val        (const std::string& id) const
    // {
    //     const auto joval =
    // }


    void                    connect_for_changes (changed_cb callback);
    std::string             dbg_string          () const;

private:
    void    signal_changed() const;

    nlohmann::json jo_;
    std::vector<changed_cb> on_changed_callbacks_;

};

} // namespace cpaf::gui::video

