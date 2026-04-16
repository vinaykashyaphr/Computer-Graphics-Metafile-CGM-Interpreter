# include "dispatchers/delimiter.hpp"
# include "param_reader.hpp"
# include "schemes/cgm_classes.hpp"
# include "utils/generic.hpp"
# include <cstddef>
# include <cstdint>
#include <cstdlib>
#include <stdexcept>
# include <iostream>




_Delimiter::_Delimiter(
    GraphicsState& state,
    const CgmElement& elem,
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>>& segment_store,
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>>& protection_regions,
    std::ofstream& log
):
    _state(state),
    _elem(elem),
    _segment_store(segment_store),
    _protection_regions(protection_regions),
    _log(log)
{}



void _Delimiter::dispatch() {

    auto id = static_cast<Delimiter>(_elem.elem_id);

    switch (id) {

        case Delimiter::NO_OP:
            break;
        
        case Delimiter::BEGIN_METAFILE:
            _begin_metafile();
            break;

        case Delimiter::END_METAFILE:
            _end_metafile();
            break;
        
        case Delimiter::BEGIN_PICTURE:
            _begin_picture();
            break;

        case Delimiter::BEGIN_PICTURE_BODY:
            _begin_picture_body();
            break;
        
        case Delimiter::END_PICTURE:
            _end_picture();
            break;
        
        case Delimiter::BEGIN_SEGMENT:
            _begin_segment();
            break;
        
        case Delimiter::END_SEGMENT:
            _end_segment();
            break;
        
        case Delimiter::BEGIN_FIGURE:
            _begin_figure();
            break;
        
        case Delimiter::END_FIGURE:
            _end_figure();
            break;
        
        case Delimiter::BEGIN_PROTECTION_REGION:
            _begin_protection_region();
            break;
        
        case Delimiter::END_PROTECTION_REGION:
            _end_protection_region();
            break;

        case Delimiter::BEGIN_COMPOUND_LINE:
            _begin_compound_line();
            break;

        case Delimiter::END_COMPOUND_LINE:
            _end_compound_line();
            break;
        
        case Delimiter::BEGIN_COMPOUND_TEXT_PATH:
            _begin_compound_text_path();
            break;

        case Delimiter::END_COMPOUND_TEXT_PATH:
            _end_compound_text_path();
            break;

        case Delimiter::BEGIN_TILE_ARRAY:
            _begin_tile_array();
            break;
        
        case Delimiter::END_TILE_ARRAY:
            _end_tile_array();
            break;
        
        case Delimiter::BEGIN_APP_STRUCTURE:
            _begin_app_structure();
            break;

        case Delimiter::BEGIN_APP_STRUCTURE_BODY:
            _begin_app_structure_body();
            break;
        
        case Delimiter::END_APP_STRUCTURE:
            _end_app_structure();
            break;

        default:
            generic_utils::log_unknown(_elem, _log);
            break;

    }

}



void _Delimiter::_begin_metafile() {

    _state.flags.in_metafile = true;

    std::size_t i = 0;

    std::string val = param_reader::read_string(_elem.params, i);
    _state.metafile_name = val;

    _log << "[CGM] METAFILE NAME: " << val << '\n';

}



void _Delimiter::_end_metafile() {

    if (_state.flags.PCS() || _state.flags.MDS()) {

        _state.flags.in_metafile = false;
        _log << "[CGM] END METAFILE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: END METAFILE");

    }

}



void _Delimiter::_begin_picture() {

    if (_state.flags.PCS() || _state.flags.MDS()) {

        _state.flags.in_picture = true;
        _state.reset_picture_state();

        std::size_t i = 0;

        std::string val = param_reader::read_string(_elem.params, i);

        _log << "[CGM] BEGIN PICTURE: " << val << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN PICTURE");

    }

}



void _Delimiter::_begin_picture_body() {

    if (_state.flags.PDS()) {

        _state.flags.in_picture_body = true;
        _log << "[CGM] BEGIN PICTURE BODY" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN PICTURE BODY");

    }

}



void _Delimiter::_end_picture() {

    if (_state.flags.POS()) {

        _state.flags.in_picture_body = false;
        _state.flags.in_picture = false;

        _log << "[CGM] END PICTURE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: END PICTURE");

    }

}



void _Delimiter::_begin_segment() {

    if (_state.flags.MDS() || _state.flags.PDS() || _state.flags.POS() || _state.flags.SOS()) {

        _state.flags.in_segment = true;

        std::size_t i = 0;
        std::uint32_t name = param_reader::read_name(_state, _elem.params, i);

        _segment_store.try_emplace(name);

        _log << "[CGM] BEGIN SEGMENT: " << name << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN SEGMENT");

    }

}



void _Delimiter::_end_segment() {

    if ((_state.flags.GSS() || _state.flags.DSS()) || _state.flags.LSS()) {

        _state.flags.in_segment = false;

        _log << "[CGM] END SEGMENT" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: END SEGMENT");

    }

}



void _Delimiter::_begin_figure() {

    if ((_state.flags.GSS() || _state.flags.DSS()) || _state.flags.POS() || _state.flags.LSS() || _state.flags.SOS()) {

        _state.flags.in_figure = true;

        _log << "[CGM] BEGIN FIGURE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN FIGURE");

    }

}



void _Delimiter::_end_figure() {

    if (_state.flags.FOS()) {

        _state.flags.in_figure = false;

        _log << "[CGM] END FIGURE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MINOR STATE FAILURE: END FIGURE");

    }

}



void _Delimiter::_begin_protection_region() {

    if ((_state.flags.GSS() || _state.flags.DSS()) || _state.flags.POS() || _state.flags.LSS() || _state.flags.SOS()) {

        _state.flags.in_protection_region = true;

        std::size_t i = 0;
        std::uint32_t index = param_reader::read_index(_state, _elem.params, i);

        _protection_regions.try_emplace(index);

        _log << "[CGM] BEGIN PROTECTION REGION: " << index << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN PROTEXTION REGION");

    }

}



void _Delimiter::_end_protection_region() {

    if (_state.flags.PRS()) {

        _state.flags.in_protection_region = false;

        _log << "[CGM] END PROTECTION REGION" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MINOR STATE FAILURE: END PROTECTION REGION");

    }

}



void _Delimiter::_begin_compound_line() {

    if ((_state.flags.GSS() || _state.flags.DSS()) || _state.flags.POS() || _state.flags.LSS() || _state.flags.SOS()) {

        _state.flags.in_compound_path = true;

        _log << "[CGM] BEGIN COMPOUND LINE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN COMPOUND LINE");

    }

}



void _Delimiter::_end_compound_line() {

    if (_state.flags.CPS()) {

        _state.flags.in_compound_path = false;

        _log << "[CGM] END COMPOUND LINE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MINOR STATE FAILURE: END COMPOUND LINE");

    }

}



void _Delimiter::_begin_compound_text_path() {

    if ((_state.flags.GSS() || _state.flags.DSS()) || _state.flags.POS() || _state.flags.LSS() || _state.flags.SOS()) {

        _state.flags.in_compound_path = true;

        _log << "[CGM] BEGIN COMPOUND TEXT PATH" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN COMPOUND TEXT PATH");

    }

}


void _Delimiter::_end_compound_text_path() {

    if (_state.flags.CPS()) {

        _state.flags.in_compound_path = false;

        _log << "[CGM] END COMPOUND TEXT PATH" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MINOR STATE FAILURE: END COMPOUND TEXT PATH");

    }

}



void _Delimiter::_begin_tile_array() {

    if (_state.flags.POS() || _state.flags.SOS()) {

        _state.flags.in_tile_array = true;

        std::size_t i = 0;
        param_reader::Point position = param_reader::read_point(_state, _elem.params, i);

        std::int32_t cell_path_direct = param_reader::read_enum(_elem.params, i);

        if (cell_path_direct < 0 || cell_path_direct > 3) {

            _log << "[ERROR] Invalid C0-19-P2: Cell Path Direction " << cell_path_direct << '\n';
            std::exit(EXIT_FAILURE);

        }

        std::int32_t line_prog_direct = param_reader::read_enum(_elem.params, i);

        if (!(line_prog_direct == 0 || line_prog_direct == 1)) {

            _log << "[ERROR] Invalid C0-19-P3: Line Progression Direction " << line_prog_direct << '\n';
            std::exit(EXIT_FAILURE);

        }

        std::int32_t num_of_tiles_p_direct = param_reader::read_int(_state, _elem.params, i);
        std::int32_t num_of_tiles_line_direct = param_reader::read_int(_state, _elem.params, i);
        std::int32_t num_of_tile_path_direct = param_reader::read_int(_state, _elem.params, i);
        std::int32_t num_of_tile_line_direct = param_reader::read_int(_state, _elem.params, i);

        double cell_size_path_direct = param_reader::read_real(_state, _elem.params, i);
        double cell_size_line_direct = param_reader::read_real(_state, _elem.params, i);

        std::int32_t img_offset_path_dir = param_reader::read_int(_state, _elem.params, i);
        std::int32_t img_offset_line_direct = param_reader::read_int(_state, _elem.params, i);

        std::int32_t img_num_cells_path_dir = param_reader::read_int(_state, _elem.params, i);
        std::int32_t img_num_cells_line_dir = param_reader::read_int(_state, _elem.params, i);

        _state.tile_array_ctx = {
            position.x,
            position.y,
            cell_path_direct,
            line_prog_direct,
            num_of_tiles_p_direct,
            num_of_tiles_line_direct,
            num_of_tile_path_direct,
            num_of_tile_line_direct,
            cell_size_path_direct,
            cell_size_line_direct,
            img_offset_path_dir,
            img_offset_line_direct,
            img_num_cells_path_dir,
            img_num_cells_line_dir
        };

        _log << "[CGM] BEGIN TILE ARRAY" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN TILE ARRAY");

    }

}



void _Delimiter::_end_tile_array() {

    if (_state.flags.TAS()) {

        _state.flags.in_tile_array = false;

        _log << "[CGM] END TILE ARRAY" << '\n';


    }

    else {

        throw std::runtime_error("[CGM] MINOR STATE FAILURE: END TILE ARRAY");

    }

}



void _Delimiter::_begin_app_structure() {

    if (_state.flags.POS() || _state.flags.SOS()) {

        _state.flags.aps_depth++;

        std::size_t i= 0;
        std::string aps_id = param_reader::read_string(_elem.params, i);
        std::string aps_type = param_reader::read_string(_elem.params, i);

        std::int32_t inheritance_flag = param_reader::read_enum(_elem.params, i);

        if (!(inheritance_flag == 0 || inheritance_flag == 1)) {

            _log << "[ERROR] Invalid C0-21-P3: Inheritance Flag " << inheritance_flag << '\n';
            std::exit(EXIT_FAILURE);

        }

        _state.aps_stack.push_back({ aps_id, aps_type, inheritance_flag });

        _log << "[CGM] BEGIN APPLICATION STRUCTURE: " << aps_id << " (" << aps_type << ")" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN APPLICATION STRUCTURE");

    }

}



void _Delimiter::_begin_app_structure_body() {

    if (_state.flags.SDS()) {

        _state.flags.aps_body_depth++;

        _log << "[CGM] BEGIN APPLICATION STRUCTURE BODY" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: BEGIN APPLICATION STRUCTURE BODY");

    }

}



void _Delimiter::_end_app_structure() {

    if (_state.flags.SOS()) {

        _state.flags.aps_body_depth--;
        _state.flags.aps_depth--;

        GraphicsState::APSEntry completed = std::move(_state.aps_stack.back());
        _state.aps_stack.pop_back();
        std::string key = completed.id;
        _state.aps_completed.emplace(std::move(key), std::move(completed));

        _log << "[CGM] END APPLICATION STRUCTURE" << '\n';

    }

    else {

        throw std::runtime_error("[CGM] MAJOR STATE FAILURE: END APPLICATION STRUCTURE");

    }

}


