
# include <cstddef>
# include <cstdint>
# include <cstring>
# include <stdexcept>
# include <string>

# include "schemes/state.hpp"
# include "param_reader.hpp"





std::int32_t param_reader::read_enum(const std::vector<std::uint8_t>& p, std::size_t& i) {

    std::int16_t v = (static_cast<std::int16_t>(p[i]) << 8) | p[i+1];
    i += 2;

    return static_cast<std::int32_t>(v);

}



std::int32_t param_reader::read_int(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.int_bytes < 1 || s.int_bytes > 4) {

        throw std::runtime_error("Malformed INTEGER PRECISION: " + std::to_string(s.int_bytes));
    }

    if (s.int_bytes == 1) {

        std::int8_t v = (static_cast<std::int8_t>(p[i]));
        i++;

        return static_cast<std::int32_t>(v);
    }

    else if (s.int_bytes == 2) {

        std::int16_t v = (static_cast<std::int16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        return static_cast<std::int32_t>(v);
    }

    else if (s.int_bytes == 3) {

        std::int32_t v = (static_cast<std::int32_t>(p[i]) << 16) |
                         (static_cast<std::int32_t>(p[i + 1]) << 8) |
                          static_cast<std::int32_t>(p[i + 2]);

        // sign extend bit 23 into bits 24-31
        if (v & 0x800000) v |= 0xFF000000;
        i += 3;

        return v;
        
    }

    else {

        std::int32_t v = (static_cast<std::int32_t>(p[i]) << 24) |
                         (static_cast<std::int32_t>(p[i+1]) << 16) |
                         (static_cast<std::int32_t>(p[i+2]) << 8) |
                          static_cast<std::int32_t>(p[i+3]);
        i += 4;

        return v;

    }

}



std::uint32_t param_reader::_read_index(int prec_bytes, const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (prec_bytes == 1) {

        return p[i++];

    }

    else if (prec_bytes == 2) {
    
        std::uint16_t v = (static_cast<std::uint16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        return static_cast<std::uint32_t>(v);

    }

    else if (prec_bytes == 3) {

        std::uint32_t v = (static_cast<std::uint32_t>(p[i]) << 16) |
                          (static_cast<std::uint32_t>(p[i + 1]) << 8) |
                           static_cast<std::uint32_t>(p[i + 2]);
        i += 3;

        return v;
        
    }

    // 4 bytes
    else {

        std::uint32_t v = (static_cast<std::uint32_t>(p[i]) << 24) |
                          (static_cast<std::uint32_t>(p[i+1]) << 16) |
                          (static_cast<std::uint32_t>(p[i+2]) << 8) |
                           static_cast<std::uint32_t>(p[i+3]);
        i += 4;

        return v;

    }

} 



std::uint32_t param_reader::read_index(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.index_bytes < 1 || s.index_bytes > 4) {

        throw std::runtime_error("Malformed INDEX PRECISION: " + std::to_string(s.index_bytes));

    }

    return _read_index(s.index_bytes, s, p, i);

}



double param_reader::_read_float(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.real_bytes == 4) {

        std::uint32_t raw = (static_cast<std::uint32_t>(p[i]) << 24) |
                            (static_cast<std::uint32_t>(p[i+1]) << 16) |
                            (static_cast<std::uint32_t>(p[i+2]) << 8) |
                             static_cast<std::uint32_t>(p[i+3]);
        i += 4;
        float v; std::memcpy(&v, &raw, 4);

        return static_cast<double>(v);

    }

    // 8 bytes
    else {

        std::uint64_t raw = (static_cast<std::uint64_t>(p[i]) << 56) |
                            (static_cast<std::uint64_t>(p[i+1]) << 48) |
                            (static_cast<std::uint64_t>(p[i+2]) << 40) |
                            (static_cast<std::uint64_t>(p[i+3]) << 32) |
                            (static_cast<std::uint64_t>(p[i+4]) << 24) |
                            (static_cast<std::uint64_t>(p[i+5]) << 16) |
                            (static_cast<std::uint64_t>(p[i+6]) << 8) |
                             static_cast<std::uint64_t>(p[i+7]);

        i += 8;
        double v; std::memcpy(&v, &raw, 8);

        return v;

    }

}


double param_reader::_read_fixed(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.real_bytes == 4) {

        std::int16_t whole = (static_cast<std::int16_t>(p[i]) << 8) | p[i+1];
        std::uint16_t frac = (static_cast<std::uint16_t>(p[i+2]) << 8) | p[i+3];
        i += 4;

        return static_cast<double>(whole) + static_cast<double>(frac) / 65536.0;

    }

    // 8 bytes
    else {

        std::int32_t whole = (static_cast<std::int32_t>(p[i]) << 24) |
                                (static_cast<std::int32_t>(p[i+1]) << 16) |
                                (static_cast<std::int32_t>(p[i+2]) << 8) |
                                static_cast<std::int32_t>(p[i+3]);

        std::uint32_t frac = (static_cast<std::uint32_t>(p[i+4]) << 24) |
                                (static_cast<std::uint32_t>(p[i+5]) << 16) |
                                (static_cast<std::uint32_t>(p[i+6]) << 8) |
                                static_cast<std::uint32_t>(p[i+7]);

        i += 8;

        return static_cast<double>(whole) + static_cast<double>(frac) / 4294967296.0;

    }
    
}


double param_reader::read_real(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.real_bytes != 4 && s.real_bytes != 8) {

        throw std::runtime_error("Malformded REAL PRECISION: " + std::to_string(s.real_bytes));
    }

    if (s.real_is_float) return param_reader::_read_float(s, p, i);
    else return param_reader::_read_fixed(s, p, i);

}



double param_reader::read_vdc(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.vdc_is_real)
        return read_real(s, p, i);

    else
        return static_cast<double>(read_int(s, p, i));

}


param_reader::Point param_reader::read_point(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    double x = read_vdc(s, p, i);
    double y = read_vdc(s, p, i);

    return {x, y};

}


std::uint32_t param_reader::read_cidx(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.cidx_bytes < 1 || s.cidx_bytes > 4) {

        throw std::runtime_error("Malformed COLOUR INDEX PRECISION: " + std::to_string(s.cidx_bytes));

    }

    return _read_index(s.cidx_bytes, s, p, i);

}


std::uint32_t param_reader::read_name(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.name_bytes < 1 || s.name_bytes > 4) {

        throw std::runtime_error("Malformed NAME PRECISION: " + std::to_string(s.name_bytes));

    }

    return _read_index(s.name_bytes, s, p, i);

}



std::string param_reader::read_string(const std::vector<std::uint8_t>& p, std::size_t& i) {

    std::string result;

    while (true) {

        std::size_t len = p[i++];
        bool more_parts = false;

        if (len == 255) {

            std::uint16_t long_len = (static_cast<std::uint16_t>(p[i]) << 8) | p[i+1];
            i += 2;

            more_parts = (long_len & 0x8000) != 0;  // bit 15
            len = long_len & 0x7FFF;                // bits 14-0

        }

        result.append(p.begin() + i, p.begin() + i + len);
        i += len;

        if (!more_parts) break;
        // if more_parts is true, loop reads the next part
    }

    return result;

}



std::uint8_t param_reader::read_colour_component(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.colour_bytes < 1 || s.colour_bytes > 4) {

        throw std::runtime_error("Malformed COLOUR PRECISION: " + std::to_string(s.colour_bytes));

    }

    std::uint32_t raw = _read_index(s.colour_bytes, s, p, i);
    std::uint64_t max_val = (1ull << (s.colour_bytes * 8)) - 1;

    return static_cast<std::uint8_t>(
        (static_cast<std::uint64_t>(raw) * 255ull) / max_val
    );

}



State::Colour::Value param_reader::read_colour_direct(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    State::Colour::Value cv;

    switch (s.colour_model) {

        // RGB
        case 1:
            cv.value = State::Colour::RGB{
                param_reader::read_colour_component(s, p, i),
                param_reader::read_colour_component(s, p, i),
                param_reader::read_colour_component(s, p, i)
            };
            break;

        // CIELAB
        case 2:
            cv.value = State::Colour::CIELAB{
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i)
            };
            break;

        // CIELUV
        case 3:
            cv.value = State::Colour::CIELUV{
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i)
            };
            break;

        // CMYK
        case 4:
            cv.value = State::Colour::CMYK{
                param_reader::read_colour_component(s, p, i),
                param_reader::read_colour_component(s, p, i),
                param_reader::read_colour_component(s, p, i),
                param_reader::read_colour_component(s, p, i)
            };
            break;

        // RGB-related
        case 5:
            cv.value = State::Colour::RGBRelated{
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i)
            };
            break;

        default:
            throw std::runtime_error(
                "Unsupported colour model: " + 
                std::to_string(s.colour_model) +
                " — registered or unknown model, cannot decode colour components"
            );

    }

    return cv;

}



State::Colour::Value param_reader::read_colour(const State& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.colour_sel_mode != 0 && s.colour_sel_mode != 1) {
        throw std::runtime_error("Invalid COLOUR SELECTION MODE: " + std::to_string(s.colour_sel_mode));
    }

    if (s.colour_sel_mode == 0) {

        std::uint32_t idx = param_reader::read_cidx(s, p, i);
        return s.get_colour(static_cast<int>(idx));

    } 

    else {

        return param_reader::read_colour_direct(s, p, i);

    }

}


