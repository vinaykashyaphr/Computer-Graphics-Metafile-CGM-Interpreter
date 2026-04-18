# pragma once

# include <cstdint>
# include <vector>
# include <string>

# include "schemes/state.hpp"




struct param_reader {

    struct Point {double x; double y;};

    // E
    static std::int32_t read_enum(const std::vector<std::uint8_t>& p, std::size_t& i);

    // I, VDC
    static std::int32_t read_int(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // R, SS, VDC
    static double read_real(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // P, SS
    static param_reader::Point read_point(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    static double read_vdc(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // CI, CO
    static std::uint32_t read_cidx(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // N
    static std::uint32_t read_name(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // IX
    static std::uint32_t read_index(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    static std::uint8_t read_colour_component(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // CO
    static State::Colour::Value read_colour_direct(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);
    static State::Colour::Value read_colour(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

    // SF
    static std::string read_string(const std::vector<std::uint8_t>& p, std::size_t& i);
    
    // nP
    // while (i + sizeof(type) <= params.size()) read one more

    private:

        static std::uint32_t _read_index(int prec_bytes, const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);
        static double _read_float(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);
        static double _read_fixed(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i);

};


