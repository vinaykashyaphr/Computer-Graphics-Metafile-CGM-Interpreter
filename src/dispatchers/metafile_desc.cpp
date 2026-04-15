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
        
        case MetafileDescriptor::INTEGER_PRECISION:
            handle_integer_precision();
            break;

        case MetafileDescriptor::REAL_PRECISION:
            handle_real_precision();
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

    std::int32_t val = param_reader::read_enum(_elem.params, i);
    _state.vdc_is_real = (val == 1);

    _log << "[CGM] VDC TYPE: " << (val == 1 ? "real" : "integer") << "\n";

}



// INTEGER PRECISION (class 1, id 4)
// params: I — one integer value containing bit width
// valid values: 8, 16, 24, 32
void _MetafileDescriptor::handle_integer_precision() {

    std::size_t i = 0;

    std::int32_t bits = param_reader::read_int(_state, _elem.params, i);
    _state.int_bytes = bits / 8;

    _log << "[CGM] INTEGER PRECISION: " << bits << " bits = " << _state.int_bytes << " bytes\n";

}



// REAL PRECISION (class 1, id 5)
// params: E, I, I — format flag, exponent bits, mantissa bits
// format: 0=floating point, 1=fixed point
// legal combos: (0,9,23)=32-bit float (0,12,52)=64-bit float
//               (1,16,16)=32-bit fixed (1,32,32)=64-bit fixed
void _MetafileDescriptor::handle_real_precision() {

    std::size_t i = 0;

    std::int32_t fmt = param_reader::read_enum(_elem.params, i);  // E reads as int (fixed 2 bytes)
    std::int32_t exp = param_reader::read_int(_state, _elem.params, i);
    std::int32_t mant = param_reader::read_int(_state, _elem.params, i);

    _state.real_is_float = (fmt == 0);
    _state.real_bytes    = (exp + mant) / 8;

    _log << "[CGM] REAL PRECISION: "
         << (fmt == 0 ? "float" : "fixed")
         << " exp=" << exp << " mant=" << mant
         << " = " << _state.real_bytes << " bytes\n";

}



// INDEX PRECISION (class 1, id 6)
// params: I — bit width
void _MetafileDescriptor::handle_index_precision() {

    std::size_t i = 0;

    std::int32_t bits = param_reader::read_int(_state, _elem.params, i);
    _state.index_bytes = bits / 8;

    _log << "[CGM] INDEX PRECISION: " << bits << " bits\n";

}



// // COLOUR PRECISION (class 1, id 7)
// // params: I — bits per colour component
// void _MetafileDescriptor::handle_colour_precision() {

//     std::size_t i = 0;

//     std::int32_t bits = param_reader::read_int(_state, _elem.params, i);
//     _state.colour_bytes = bits / 8;

//     _log << "[CGM] COLOUR PRECISION: " << bits << " bits\n";

// }


