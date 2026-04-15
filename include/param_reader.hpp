# pragma once

# include <cstdint>
# include <vector>

# include "schemes/graphics_state.hpp"




struct param_reader {

    struct Point {double x; double y;};

    // E
    static std::int32_t read_enum(const std::vector<std::uint8_t>& p, std::size_t& i);

    // I, VDC
    static std::int32_t read_int(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // R, SS, VDC
    static double read_real(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // P, SS
    static param_reader::Point read_point(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    static double read_vdc(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // CI, CO
    static std::uint32_t read_cidx(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // N
    static std::uint32_t read_name(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // IX
    static std::uint32_t read_index(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    static std::uint8_t read_colour_component(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // CO
    static GraphicsState::ColourValue read_colour_direct(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    static GraphicsState::ColourValue read_colour(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // SF
    static std::string read_string(const std::vector<std::uint8_t>& p, std::size_t& i);
    
    // nP
    // while (i + sizeof(type) <= params.size()) read one more

    private:

        static std::uint32_t _read_index(int prec_bytes, const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
        static double _read_float(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);
        static double _read_fixed(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i);

};


