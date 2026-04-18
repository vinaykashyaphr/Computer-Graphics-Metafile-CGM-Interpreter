# pragma once

# include <string>



struct Node {

    virtual ~Node() = default;
    virtual std::string to_svg() const = 0;  // each node writes itself to SVG

};


