
# include <cstdint>
# include <cstring>

# include "schemes/graphics_state.hpp"
# include "param_reader.hpp"




std::int32_t param_reader::read_int(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.int_bytes == 2) {

        std::int16_t v = (static_cast<std::int16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        return static_cast<std::int32_t>(v);
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



std::uint32_t param_reader::read_index(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.index_bytes == 2) {
    
        std::uint16_t v = (static_cast<std::uint16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        return static_cast<std::uint32_t>(v);

    } 
    
    else {

        std::uint32_t v = (static_cast<std::uint32_t>(p[i]) << 24) |
                          (static_cast<std::uint32_t>(p[i+1]) << 16) |
                          (static_cast<std::uint32_t>(p[i+2]) << 8) |
                           static_cast<std::uint32_t>(p[i+3]);
        i += 4;

        return v;

    }

}



double param_reader::_read_float(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.real_bytes == 4) {

        std::uint32_t raw = (static_cast<std::uint32_t>(p[i]) << 24) |
                            (static_cast<std::uint32_t>(p[i+1]) << 16) |
                            (static_cast<std::uint32_t>(p[i+2]) << 8) |
                             static_cast<std::uint32_t>(p[i+3]);
        i += 4;
        float v; std::memcpy(&v, &raw, 4);

        return static_cast<double>(v);

    }

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


double param_reader::_read_fixed(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.real_bytes == 4) {

        std::int16_t whole = (static_cast<std::int16_t>(p[i]) << 8) | p[i+1];
        std::uint16_t frac = (static_cast<std::uint16_t>(p[i+2]) << 8) | p[i+3];
        i += 4;

        return static_cast<double>(whole) + static_cast<double>(frac) / 65536.0;

    }

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


double param_reader::read_real(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.real_is_float) return param_reader::_read_float(s, p, i);
    else return param_reader::_read_fixed(s, p, i);

}



double param_reader::read_point(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.vdc_is_real)
        return read_real(s, p, i);

    else
        return static_cast<double>(read_int(s, p, i));

}



std::uint32_t param_reader::read_cidx(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.cidx_bytes == 1)
        return p[i++];

    else {

        std::uint16_t v = (static_cast<std::uint16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        return static_cast<std::uint32_t>(v);

    }

}


std::uint32_t param_reader::read_name(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    if (s.name_bytes == 2) {

        std::uint16_t v = (static_cast<std::uint16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        return static_cast<std::uint32_t>(v);

    } 

    else {

        std::uint32_t v = (static_cast<std::uint32_t>(p[i])   << 24) |
                          (static_cast<std::uint32_t>(p[i+1]) << 16) |
                          (static_cast<std::uint32_t>(p[i+2]) <<  8) |
                           static_cast<std::uint32_t>(p[i+3]);
        i += 4;

        return v;
    }
}



std::string param_reader::read_string(const std::vector<std::uint8_t>& p, std::size_t& i) {

    std::size_t len = p[i++];

    if (len == 255) {

        std::uint16_t long_len = (static_cast<std::uint16_t>(p[i]) << 8) | p[i+1];
        i += 2;

        len = long_len & 0x7FFF;

    }

    std::string s(p.begin() + i, p.begin() + i + len);
    i += len;

    return s;

}



std::uint8_t param_reader::read_colour_component(
    const std::vector<std::uint8_t>& p, std::size_t& i)
{
    // colour_bytes is bits/8 — currently always 1 byte (8-bit) per spec defaults
    // extend here if colour_bytes == 2 support needed
    return p[i++];
}



GraphicsState::ColourValue param_reader::read_colour_direct(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {

    GraphicsState::ColourValue cv;

    switch (s.colour_model) {

        // RGB
        case 1:
            cv.value = GraphicsState::ColourRGB{
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i)
            };
            break;

        // CIELAB
        case 2:
            cv.value = GraphicsState::ColourCIELAB{
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i)
            };
            break;

        // CIELUV
        case 3:
            cv.value = GraphicsState::ColourCIELUV{
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i)
            };
            break;

        // CMYK
        case 4:

            cv.value = GraphicsState::ColourCMYK{
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i)
            };
            break;

        // RGB-related
        case 5:
            cv.value = GraphicsState::ColourRGBRelated{
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i),
                param_reader::read_real(s, p, i)
            };
            break;

        default:
            cv.value = GraphicsState::ColourRGB{
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i),
                param_reader::read_colour_component(p, i)
            };
            break;

    }

    return cv;

}



GraphicsState::ColourValue param_reader::read_colour(const GraphicsState& s, const std::vector<std::uint8_t>& p, std::size_t& i) {
    if (s.colour_sel_mode == 0) {

        std::uint32_t idx = param_reader::read_cidx(s, p, i);

        return GraphicsState::ColourValue {

            GraphicsState::ColourRGB{

                static_cast<std::uint8_t>(idx & 0xFF), 0, 0
            }

        };

    } 
    
    else {

        return param_reader::read_colour_direct(s, p, i);

    }

}