# include "dispatchers/delimiter.hpp"
# include "schemes/cgm_classes.hpp"
# include "utils/generic.hpp"




_Delimiter::_Delimiter(GraphicsState& state, const CgmElement& elem, std::ofstream& log):
    _state(state),
    _elem(elem),
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
            break;
        
        case Delimiter::BEGIN_PICTURE:
            break;
        
        case Delimiter::BEGIN_PICTURE_BODY:
            break;
        
        case Delimiter::END_PICTURE:
            break;
        
        case Delimiter::BEGIN_SEGMENT:
            break;
        
        case Delimiter::END_SEGMENT:
            break;
        
        case Delimiter::BEGIN_FIGURE:
            break;
        
        case Delimiter::END_FIGURE:
            break;
        
        case Delimiter::BEGIN_PROTECTION_REGION:
            break;
        
        case Delimiter::END_PROTECTION_REGION:
            break;

        case Delimiter::BEGIN_COMPOUND_LINE:
            break;

        case Delimiter::END_COMPOUND_LINE:
            break;
        
        case Delimiter::BEGIN_COMPOUND_TEXT_PATH:
            break;

        case Delimiter::END_COMPOUND_TEXT_PATH:
            break;

        case Delimiter::BEGIN_TILE_ARRAY:
            break;
        
        case Delimiter::BEGIN_APP_STRUCTURE:
            break;

        case Delimiter::BEGIN_APP_STRUCTURE_BODY:
            break;
        
        case Delimiter::END_APP_STRUCTURE:
            break;

        default:
            generic_utils::log_unknown(_elem, _log);
            break;

    }

}



void _Delimiter::_begin_metafile() {


}


