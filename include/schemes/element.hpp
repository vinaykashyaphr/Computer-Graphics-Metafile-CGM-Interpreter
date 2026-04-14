# pragma once

# include <cstdint>
# include <vector>




struct CgmElement {

    std::uint8_t elem_class;
    std::uint8_t elem_id;
    std::vector<std::uint8_t> params;

};


