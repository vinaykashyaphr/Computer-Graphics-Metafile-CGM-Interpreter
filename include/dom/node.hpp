# pragma once

# include "schemes/graphics_state.hpp"



struct Node {

    GraphicsState::Snapshot state;  // frozen style at creation time
    virtual ~Node() = default;
    virtual std::string to_svg() const = 0;  // each node writes itself to SVG

};


