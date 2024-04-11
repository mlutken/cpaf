#pragma once

#include <functional>
#include <vector>
#include <chrono>
#include <nlohmann/json.hpp>
#include <cpaf_libs/gui/color.h>

namespace cpaf::gui::video {

class config
{
public:
    using changed_cb = std::function<void()>;

    config();
    void                        set                     (const nlohmann::json& jo);

    const nlohmann::json&       json                    () const { return jo_;  }
    const nlohmann::json&       controls                () const { return jo_["controls"];  }
    void                        controls_set            (const std::string& id, const nlohmann::json& val);

    std::string                 str                     (const std::string& group, const std::string& id) const;
    std::int32_t                int32                   (const std::string& group, const std::string& id) const;
    bool                        bool_val                (const std::string& group, const std::string& id) const;
    float                       float_val               (const std::string& group, const std::string& id) const;
    cpaf::gui::color            color                   (const std::string& group, const std::string& id) const;
    cpaf::gui::color            color_rgba              (const std::string& group, const std::string& id) const;
    std::chrono::seconds        seconds                 (const std::string& group, const std::string& id) const;

    void                        str_set                 (const std::string& group, const std::string& id, const std::string& val);
    void                        int32_set               (const std::string& group, const std::string& id, int32_t val);
    void                        float_set               (const std::string& group, const std::string& id, float val);
    void                        color_set               (const std::string& group, const std::string& id, cpaf::gui::color val);
    void                        seconds_set             (const std::string& group, const std::string& id, std::chrono::seconds val);

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

