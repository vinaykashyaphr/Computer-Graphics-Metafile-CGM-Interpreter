# pragma once

# include <cstdint>
# include <vector>

# include "schemes/graphics_state.hpp"




namespace param_reader {

    // E, I, VDC
    std::int32_t read_int(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // R, SS, VDC
    double read_real(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    double _read_float(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    double _read_fixed(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // P, SS
    double read_point(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // CI, CO
    std::uint32_t read_cidx(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // N
    std::uint32_t read_name(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // IX
    std::uint32_t read_index(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    std::uint8_t read_colour_component(const std::vector<std::uint8_t>& p, std::size_t& i);

    // CO
    GraphicsState::ColourValue read_colour_direct(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    GraphicsState::ColourValue read_colour(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // SF
    std::string read_string(const std::vector<std::uint8_t>& p, std::size_t& i);

    // nP
    // while (i + sizeof(type) <= params.size()) read one more

}


