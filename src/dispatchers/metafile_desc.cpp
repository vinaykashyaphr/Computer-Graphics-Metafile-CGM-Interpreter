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

        case MetafileDescriptor::NAME_PRECISION:
            _name_precision();
            break;

        case MetafileDescriptor::MAX_VDC_EXTENT:
            _maximum_vdc_extent();
            break;

        case MetafileDescriptor::SEGMENT_PRIORITY_EXTENT:
            _segment_priority_extent();
            break;

        case MetafileDescriptor::COLOUR_MODEL:
            _colour_model();
            break;

        case MetafileDescriptor::COLOUR_CALIBRATION:
            _colour_calibration();
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



void _MetafileDescriptor::_name_precision() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: NAME PRECISION");

    }

    std::size_t i = 0;
    std::int32_t name_prec = param_reader::read_int(_state, _elem.params, i);

    if (name_prec != 8 && name_prec != 16 && name_prec != 24 && name_prec != 32) {

        throw std::runtime_error("[CGM] Invalid NAME PRECISION: " + std::to_string(name_prec));

    }

    _state.name_bytes = static_cast<int>(name_prec);
    _log << "[CGM] NAME PRECISON: " << name_prec << '\n';

}



void _MetafileDescriptor::_maximum_vdc_extent() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: MAXIMUM VDC EXTENT");

    }

    std::size_t i = 0;
    auto p1 =  param_reader::read_point(_state, _elem.params, i);
    auto p2 = param_reader::read_point(_state, _elem.params, i);

    _state.max_vdc_extent = { p1.x, p1.y, p2.x, p2.y };

    _log << "[CGM] MAXIMUM VDC EXTENT:"
         << " (" << p1.x << "," << p1.y << ")"
         << " (" << p2.x << "," << p2.y << ")\n";

}



void _MetafileDescriptor::_segment_priority_extent() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: SEGMENT PRIORITY EXTENT");

    }

    std::size_t i = 0;
    std::int32_t min_priority = param_reader::read_int(_state, _elem.params, i);
    std::int32_t max_priority = param_reader::read_int(_state, _elem.params, i);


    if (min_priority < 0 || max_priority < 0) {
        throw std::runtime_error("[CGM] SEGMENT PRIORITY EXTENT: values must be non-negative");
    }


    if (min_priority >= max_priority) {
        throw std::runtime_error("[CGM] SEGMENT PRIORITY EXTENT: min must be less than max");
    }

    _state.segment_priority_min = static_cast<int>(min_priority);
    _state.segment_priority_max = static_cast<int>(max_priority);

    _log << "[CGM] SEGMENT PRIORITY EXTENT: (" << min_priority << ", " << max_priority << ")\n";

}



void _MetafileDescriptor::_colour_model() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: COLOUR MODEL");

    }

    std::size_t i = 0;
    std::uint32_t model = param_reader::read_index(_state, _elem.params, i);

    if (model < 1 || model > 5) {

        throw std::runtime_error("[CGM] Invalid COLOUR MODEL: " + std::to_string(model));

    }

    _state.colour_model = static_cast<int>(model);

    _log << "[CGM] COLOUR MODEL: " << model << " ("
         << (model == 1 ? "RGB" :
             model == 2 ? "CIELAB" :
             model == 3 ? "CIELUV" :
             model == 4 ? "CMYK" : "RGB-related")
         << ")\n";

}



void _MetafileDescriptor::_colour_calibration() {

    if (!_state.flags.MDS()) {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: COLOUR CALIBRATION");

    }

    _log << "[CGM] COLOUR CALIBRATION:";

    std::size_t i = 0;

    std::uint32_t calibration_selection = param_reader::read_index(_state, _elem.params, i);

    if (calibration_selection > 9 || calibration_selection < 1) {

        throw std::runtime_error("[CGM] Invalid CALIBRATION SELECTION: " + std::to_string(calibration_selection));
    }

    // 2 = "reference white only"                   → P2, P3, P4
    // 3 = "reference white, matrix1"               → P2, P3, P4, P5
    // 4 = "reference white, matrix1, LUT"          → P2, P3, P4, P5, P7, P8, P9, P10
    // 5 = "reference white, matrix1, LUT, matrix2" → P2, P3, P4, P5, P6, P7, P8, P9, P10
    // 6 = "reference white, matrix1, matrix2"      → P2, P3, P4, P5, P6
    // 7 = "LUT, matrix2"                           → P6, P7, P8, P9, P10
    // 8 = "matrix2"                                → P6
    // 9 = "reference white, grid"                  → P2, P3, P4, P11, P12, P13

    _log << "\n\tP1: " << calibration_selection;

    if (
        calibration_selection == 2 || calibration_selection == 3 || calibration_selection == 4 ||
        calibration_selection == 5 || calibration_selection == 6 || calibration_selection == 9
    ) {

        // p2, p3, p4
        _state.colour_calibration.Xn = param_reader::read_real(_state, _elem.params, i);
        _state.colour_calibration.Yn = param_reader::read_real(_state, _elem.params, i);
        _state.colour_calibration.Zn = param_reader::read_real(_state, _elem.params, i);
        
        _log << 
             "\n\tP2: " << _state.colour_calibration.Xn <<
             "\n\tP3: " << _state.colour_calibration.Yn <<
             "\n\tP4: " << _state.colour_calibration.Zn <<
             '\n';
    }


    if (
        calibration_selection == 3 || calibration_selection == 4 ||
        calibration_selection == 5 || calibration_selection == 6
    ) {

        // add p5
        for (int n = 0; n < 9; n++) {

            _state.colour_calibration.matrix1[n] = param_reader::read_real(_state, _elem.params, i);

            _log << "\n\tP5: " << _state.colour_calibration.matrix1[n] << ' ';

        }

        _log << '\n';

    }


    if (
        calibration_selection == 5 || calibration_selection == 6 ||
        calibration_selection == 7 || calibration_selection == 8
    ) {

        // add p6
        for (int n = 0; n < 9; n++) {

            _state.colour_calibration.matrix2[n] = param_reader::read_real(_state, _elem.params, i);

            _log << "\n\tP6: " << _state.colour_calibration.matrix2[n] << ' ';

        }

        _log << '\n';

    }


    if (calibration_selection == 4 || calibration_selection == 5 || calibration_selection == 7) {

        // add p7, p8, p9, p10
        std::int32_t lut_n = param_reader::read_int(_state, _elem.params, i);
        _state.colour_calibration.lut_size = lut_n;

        _log << "\n\tP7: " << lut_n << '\n';

        for (int n = 0; n < lut_n; n++)
            _state.colour_calibration.lut_r.push_back({
                param_reader::read_colour_component(_state, _elem.params, i),
                param_reader::read_colour_component(_state, _elem.params, i)
            });

        for (int n = 0; n < lut_n; n++)
            _state.colour_calibration.lut_g.push_back({
                param_reader::read_colour_component(_state, _elem.params, i),
                param_reader::read_colour_component(_state, _elem.params, i)
            });

        for (int n = 0; n < lut_n; n++)
            _state.colour_calibration.lut_b.push_back({
                param_reader::read_colour_component(_state, _elem.params, i),
                param_reader::read_colour_component(_state, _elem.params, i)
            });

    }


    if (calibration_selection == 9) {

        // add p11, p12, p13
        std::int32_t m = param_reader::read_int(_state, _elem.params, i);
        _state.colour_calibration.grid_size = m;

        _log << "\n\tP11: " << m << '\n';

        for (int n = 0; n < m; n++)
            _state.colour_calibration.cmyk_grid_locations.push_back(
                param_reader::read_colour_direct(_state, _elem.params, i)
            );

        for (int n = 0; n < m; n++) 
            _state.colour_calibration.cmyk_grid_xyz.push_back({
                param_reader::read_real(_state, _elem.params, i),
                param_reader::read_real(_state, _elem.params, i),
                param_reader::read_real(_state, _elem.params, i)
            });

    }

}



