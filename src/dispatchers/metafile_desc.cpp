# include "dispatchers/metafile_desc.hpp"
// #include "reader.hpp"

#include "reader.hpp"
# include "schemes/cgm_classes.hpp"
#include "schemes/element.hpp"
#include "schemes/state.hpp"
# include "utils/generic.hpp"
# include "param_reader.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>




_MetafileDescriptor::_MetafileDescriptor(State& state, const CgmElement& elem, std::ofstream& log):
    _elem(elem),
    _state(state),
    _log(log)
{}



void _MetafileDescriptor::dispatch() {

    auto id = static_cast<MetafileDescriptor>(_elem.elem_id);

    switch (id) {

        case MetafileDescriptor::METAFILE_VERSION:
            _metafile_version();
            break;

        case MetafileDescriptor::METAFILE_DESCRIPTION:
            _metafile_desc();
            break;

        case MetafileDescriptor::VDC_TYPE:
            _vdc_type();
            break;
        
        case MetafileDescriptor::INTEGER_PRECISION:
            _integer_precision();
            break;

        case MetafileDescriptor::REAL_PRECISION:
            _real_precision();
            break;

        case MetafileDescriptor::INDEX_PRECISION:
            _index_precision();
            break;

        case MetafileDescriptor::COLOUR_PRECISION:
            _colour_precision();
            break;

        case MetafileDescriptor::COLOUR_INDEX_PRECISION:
            _colour_index_precision();
            break;

        case MetafileDescriptor::MAX_COLOUR_INDEX:
            _max_colour_index();
            break;

        case MetafileDescriptor::COLOUR_VALUE_EXTENT:
            _colour_value_extent();
            break;
        
        case MetafileDescriptor::METAFILE_ELEMENT_LIST:
            _metafile_element_list();
            break;
        
        case MetafileDescriptor::METAFILE_DEFAULTS_REPL:
            _metafile_defaults_replacement();
            break;

        case MetafileDescriptor::FONT_LIST:
            _font_list();
            break;

        case MetafileDescriptor::CHAR_SET_LIST:
            _character_set_list();
        break;

        case MetafileDescriptor::CHAR_CODING_ANNOUNCER:
            _character_coding_announcer();
        break;

        default:
            generic_utils::log_unknown(_elem, _log);
            break;

    }

}




void _MetafileDescriptor::_metafile_version() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: METAFILE VERSION");

    }

    std::size_t i = 0;
    std::int32_t version = param_reader::read_int(_state, _elem.params, i);

    if (version > 4 || version < 1) {
        throw std::runtime_error(
            "[CGM] MAJOR STATE FAILURE: Invalid Metafile Version" + std::to_string(version)
        );
    }

    _state.metafile_version = static_cast<int>(version);

    _log << "[CGM] METAFILE VERSION: " << version << '\n';

}



void _MetafileDescriptor::_metafile_desc() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: METAFILE DESCRIPTION");

    }

    std::size_t i = 0;
    std::string desc = param_reader::read_string(_elem.params, i);

    _state.metafile_desc = desc;

    _log << "[CGM] METAFILE DESCRIPTION: " << desc << '\n';

}



void _MetafileDescriptor::_vdc_type() {


    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: VDC TYPE");

    }

    std::size_t i = 0;

    std::int32_t val = param_reader::read_enum(_elem.params, i);

    if (val != 0 && val != 1) {

        throw std::runtime_error("[CGM] Invalid VDC TYPE Value: " + std::to_string(val));

    }

    _state.vdc_is_real = (val == 1);

    _log << "[CGM] VDC TYPE: " << (val == 1 ? "real" : "integer") << "\n";

}



void _MetafileDescriptor::_integer_precision() {


    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: INTEGER PRECISION");

    }

    std::size_t i = 0;
    std::int32_t bits = param_reader::read_int(_state, _elem.params, i);

    if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {

        throw std::runtime_error("[CGM] Invalid INTEGER PRECISION: " + std::to_string(bits));

    }

    _state.int_bytes = static_cast<int>(bits / 8);
    _log << "[CGM] INTEGER PRECISION: " << bits << '\n';

}



void _MetafileDescriptor::_real_precision() {


    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: REAL PRECISION");

    }

    std::size_t i = 0;

    // {fmt, exp, mant}
    std::array<std::int32_t, 3> real_prec = {
        param_reader::read_enum(_elem.params, i),
        param_reader::read_int(_state, _elem.params, i),
        param_reader::read_int(_state, _elem.params, i)

    };

    if (

        (real_prec != std::array<int32_t, 3> {0, 9, 23}) &&
        (real_prec != std::array<int32_t, 3> {0, 12, 52}) &&
        (real_prec != std::array<int32_t, 3> {1, 16, 16}) &&
        (real_prec != std::array<int32_t, 3> {1, 32, 32})

    ) {

        throw std::runtime_error(

            "[CGM] Invalid REAL PRECISION: " + 
            std::to_string(real_prec.at(0)) + " " +
            std::to_string(real_prec.at(1)) + " " +
            std::to_string(real_prec.at(2))

        );

    }

    _state.real_is_float = (real_prec.at(0) == 0);
    _state.real_bytes    = static_cast<int>((real_prec.at(1) + real_prec.at(2)) / 8);

    _log << "[CGM] REAL PRECISION: "
         << (real_prec.at(0) == 0 ? "float" : "fixed")
         << "\n\t" << real_prec.at(1)
         << "\n\t" << real_prec.at(2)
         << '\n';

}



void _MetafileDescriptor::_index_precision() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: INDEX PRECISION");

    }

    std::size_t i = 0;
    std::int32_t bits = param_reader::read_int(_state, _elem.params, i);

    if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {

        throw std::runtime_error("[CGM] Invalid INDEX PRECISION: " + std::to_string(bits));

    }

    _state.index_bytes = static_cast<int>(bits / 8);
    _log << "[CGM] INDEX PRECISION: " << bits << " bits\n";

}



void _MetafileDescriptor::_colour_precision() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: COLOUR PRECISION");

    }

    std::size_t i = 0;
    std::int32_t bits = param_reader::read_int(_state, _elem.params, i);

    if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {

        throw std::runtime_error("[CGM] Invalid COLOUR PRECISION: " + std::to_string(bits));

    }

    _state.colour_bytes = static_cast<int>(bits / 8);
    _log << "[CGM] COLOUR PRECISION: " << bits << " bits\n";

}



void _MetafileDescriptor::_colour_index_precision() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: COLOUR INDEX PRECISION");

    }

    std::size_t i = 0;
    std::int32_t bits = param_reader::read_int(_state, _elem.params, i);

    if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {

        throw std::runtime_error("[CGM] Invalid COLOUR INDEX PRECISION: " + std::to_string(bits));

    }

    _state.cidx_bytes = static_cast<int>(bits / 8);
    _log << "[CGM] COLOUR INDEX PRECISION: " << bits << '\n';

}



void _MetafileDescriptor::_max_colour_index() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: MAXIMUM COLOUR INDEX");

    }

    std::size_t i = 0;
    std::uint32_t index = param_reader::read_cidx(_state, _elem.params, i);

    _state.max_colour_index = static_cast<int>(index);
    _log << "[CGM] MAXIMUM COLOUR INDEX: " << index << '\n';

}



void _MetafileDescriptor::_colour_value_extent() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: COLOUR VALUE EXTENT");

    }

    std::size_t i = 0;

    _log << "[CGM] COLOUR VALUE EXTENT:";

    if (_state.colour_model == 1 || _state.colour_model == 4) {

        State::Colour::Value min_colour = param_reader::read_colour_direct(_state, _elem.params, i);
        State::Colour::Value max_colour = param_reader::read_colour_direct(_state, _elem.params, i);

        if (_state.colour_model == 1) {

            const auto& min_rgb = std::get<State::Colour::RGB>(min_colour.value);
            const auto& max_rgb = std::get<State::Colour::RGB>(max_colour.value);

            _state.colour_value_extent.value = std::make_pair(min_rgb, max_rgb);

            _log << "\n\t" << std::to_string(min_rgb.r);
            _log << " " << std::to_string(min_rgb.g);
            _log << " " << std::to_string(min_rgb.b);

            _log << "\n\t" << std::to_string(max_rgb.r);
            _log << " " << std::to_string(max_rgb.g);
            _log << " " << std::to_string(max_rgb.b);

            _log << '\n';
        }


        else {

            const auto& min_cmyk = std::get<State::Colour::CMYK>(min_colour.value);
            const auto& max_cmyk = std::get<State::Colour::CMYK>(max_colour.value);

            _state.colour_value_extent.value = std::make_pair(min_cmyk, max_cmyk);

            _log << "\n\t" << min_cmyk.c;
            _log << " " << min_cmyk.m;
            _log << " " << min_cmyk.y;
            _log << " " << min_cmyk.k;

            _log << "\n\t" << max_cmyk.c;
            _log << " " << max_cmyk.m;
            _log << " " << max_cmyk.y;
            _log << " " << max_cmyk.k;

            _log << '\n';

        }

    }

    else if (_state.colour_model == 2 || _state.colour_model == 3 || _state.colour_model == 5) {

        std::array<State::Colour::ScaleOffset, 3> extents;

        extents[0].scale  = param_reader::read_real(_state, _elem.params, i);
        extents[0].offset = param_reader::read_real(_state, _elem.params, i);

        extents[1].scale  = param_reader::read_real(_state, _elem.params, i);
        extents[1].offset = param_reader::read_real(_state, _elem.params, i);

        extents[2].scale  = param_reader::read_real(_state, _elem.params, i);
        extents[2].offset = param_reader::read_real(_state, _elem.params, i);

        _state.colour_value_extent.value = extents;

        for (int n = 0; n < 3; n++) {
            _log << extents[n].scale << " " << extents[n].offset;
        }

        _log << '\n';

    }

    else {

        throw std::runtime_error("[CGM] Invalid COLOUR VALUE EXTENT");

    }

}


void _MetafileDescriptor::_metafile_element_list() {

    if (!_state.flags.MDS()) {
        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: METAFILE ELEMENT LIST");
    }

    // The MEL is informational — it declares which elements appear in this file.
    // A strict interpreter could validate that it supports all listed elements
    // before proceeding. For a converter, log and continue.

    _log << "[CGM] METAFILE ELEMENT LIST (informational — not parsed)\n";

}



void _MetafileDescriptor::_metafile_defaults_replacement() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: METAFILE DEFAULTS REPLACEMENT");

    }

    _state.flags.in_defaults_replacement = true;

    Reader sub_reader(_elem.params);
    _state.metafile_defaults = sub_reader.take_elements();

    _state.flags.in_defaults_replacement = false;

    _log << "[CGM] METAFILE DEFAULT REPLACEMENT:" 
         << _state.metafile_defaults.size()
         << '\n';

}



void _MetafileDescriptor::_font_list() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: FONT LIST");

    }

    std::size_t i = 0;
    _state.font_list.clear();

    while (i < _elem.params.size()) {

        std::string name = param_reader::read_string(_elem.params, i);
        _state.font_list.push_back(name);

        _log << "[CGM] FONT LIST [" << _state.font_list.size() << "]: " 
             << _state.font_list.back() << '\n';

    }

}



void _MetafileDescriptor::_character_set_list() {

    if (!_state.flags.MDS()) {
        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: CHARACTER SET LIST");
    }

    std::size_t i = 0;
    _state.char_set_list.clear();

    while (i < _elem.params.size()) {

        std::int32_t type = param_reader::read_enum(_elem.params, i);

        if (type < 0 || type > 4) {
            throw std::runtime_error("[CGM] Invalid CHARACTER SET TYPE: " + std::to_string(type));
        }

        std::string designation = param_reader::read_string(_elem.params, i);

        _state.char_set_list.push_back(
            {static_cast<int>(type), std::move(designation)}
        );

        _log << "[CGM] CHARACTER SET LIST [" << _state.char_set_list.size() << "]:"
             << "\n\t" << type
             << "\n\t" << _state.char_set_list.back().designation
             << '\n';

    }

}



void _MetafileDescriptor::_character_coding_announcer() {

    if (!_state.flags.MDS()) {
        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: CHARACTER CODING ANNOUNCER");
    }

    std::size_t i = 0;
    std::int32_t announcer = param_reader::read_enum(_elem.params, i);

    if (announcer < 0 || announcer > 3) {
        throw std::runtime_error("[CGM] Invalid CHARACTER CODING ANNOUNCER: " + std::to_string(announcer));
    }

    _state.char_coding_announcer = static_cast<int>(announcer);

    _log << "[CGM] CHARACTER CODING ANNOUNCER: " << announcer << '\n';

}



