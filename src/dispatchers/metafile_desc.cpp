# include "dispatchers/metafile_desc.hpp"
# include "schemes/cgm_classes.hpp"
# include "utils/generic.hpp"
# include "param_reader.hpp"




_MetafileDescriptor::_MetafileDescriptor(GraphicsState& state, const CgmElement& elem, std::ofstream& log):
    _elem(elem),
    _state(state),
    _log(log)
{}



void _MetafileDescriptor::dispatch() {

    auto id = static_cast<MetafileDescriptor>(_elem.elem_id);

    switch (id) {

        case MetafileDescriptor::VDC_TYPE:
            handle_vdc_type();
            break;

        default:
            generic_utils::log_unknown(_elem, _log);
            break;

    }

}




// VDC TYPE (class 1, id 3)
// params: E — one 2-byte enumerated value
// 0 = integer coords, 1 = real coords
void _MetafileDescriptor::handle_vdc_type() {

    std::size_t i = 0;

    std::int32_t val = param_reader::read_int(_state, _elem.params, i);
    _state.vdc_is_real = (val == 1);

    _log << "[CGM] VDC TYPE: " << (val == 1 ? "real" : "integer") << "\n";

}
