# include "schemes/state.hpp"
#include <optional>




State::Colour::Value State::get_colour(int index) const {

    auto it = colour_table.find(index);
    if (it != colour_table.end())
        return it->second;

    // index not in table — file has not defined it yet
    // fall back to device-dependent defaults per spec
    if (index == 0)
        return Colour::Value{ Colour::RGB{255, 255, 255} };  // white background

    else
        return Colour::Value{ Colour::RGB{0, 0, 0} };        // black foreground

}



void State::reset_picture_state() {

    colour_sel_mode = 0;
    colour_table.clear();

    protection_regions.clear();

    aps_stack.clear();
    aps_completed.clear();

    current_picture_name = std::nullopt;
    
    std::erase_if(segment_store, [](const auto& entry) {
        return !entry.second.is_global;
    });
}


