# include "reader.hpp"



struct ElementReader::ElementProps {

    std::uint8_t  elem_class;
    std::uint8_t  elem_id;
    std::uint16_t param_len;

};



ElementReader::ElementReader(const std::vector<std::uint8_t>& data):
    _data(data)
{
    read();
}

ElementReader::~ElementReader() = default;



ElementReader::ElementProps ElementReader::get_props(std::size_t& elem_index) {

    // CGM is BIG-ENDIAN, combining 2 bytes into one
    std::uint16_t word = (
        (static_cast<std::uint16_t>(_data.at(elem_index)) << 8) | _data.at(elem_index + 1)
    );

    elem_index += 2;

    // AAAA BBBB CCCC DDDD
    // bits 15-12 = element class: 0000 0000 0000 AAAA 
    // bits 11-5  = element id: 0000 0AAA ABBB BCCC -> 0000 0000 0BBB BCCC
    // bits 4-0   = param length: 0000 0000 000C DDDD

    std::uint8_t elem_class = (word >> 12) & 0x0F;
    std::uint8_t elem_id = (word >> 5) & 0x7F;
    std::uint16_t param_len = word & 0x1F;

    return {elem_class, elem_id, param_len};
}



// standard defines it like this:

// If the 5-bit parameter list length field contains the value 31 (11111 binary), this is defined as the "long form"
// The actual parameter list length is carried in the next 16-bit word
// Bit 15 of that word is the "not last partition" flag
// Bits 14–0 of that word carry the actual length
bool ElementReader::is_partitioned(std::uint16_t& param_len, std::size_t& elem_index) {

    bool partitioned = false;

    if (param_len == 31) {

        std::uint16_t long_word = (
            (static_cast<std::uint16_t>(_data.at(elem_index)) << 8) | _data.at(elem_index + 1)
        );

        elem_index += 2;

        // Bit 15 of the long word = partition flag.
        // 1 = "this is not the last fragment, more follow (eg: splines divided into different fragmnents)"
        // 0 = "this is the last (or only) fragment"
        partitioned = (long_word & 0x8000) != 0;

        // The actual length is bits 14-0 (mask off bit 15)
        param_len = long_word & 0x7FFF;

    }

    return partitioned;

}


void ElementReader::interpret_elems(bool is_continuation, const auto& elem_props, std::size_t& elem_index) {

    // Partition reassembly
    // If this element is a continuation of a previous fragment,
    // the last element in our list is the same element_class+id.
    // We just append the new bytes to its params instead of adding a new entry.
    if (is_continuation && !_elements.empty()) {

        // This fragment belongs to the previous element — append its bytes
        auto& prev_elem = _elements.back();
        prev_elem.params.insert(
            prev_elem.params.end(), 
            _data.begin() + elem_index, 
            _data.begin() + elem_index + elem_props.param_len
        );

    }

    else {

        // Normal case: create a new element
        CgmElement element;
        element.elem_class = elem_props.elem_class;
        element.elem_id = elem_props.elem_id;
        element.params.assign(_data.begin() + elem_index, _data.begin() + elem_index + elem_props.param_len);
        _elements.push_back(std::move(element));

    }

    elem_index += elem_props.param_len;

    // CGM always pads to even byte boundaries.
    // If the param length was odd, there is 1 silent padding byte to skip.
    if (elem_props.param_len % 2 != 0) {
        elem_index += 1;
    }

}



void ElementReader::read() {

    std::size_t elem_index = 0;
    bool in_partition = false;

    while (elem_index + 2 <= _data.size()) {

        auto elem_props = get_props(elem_index);

        if (elem_props.param_len == 31 && elem_index + 2 > _data.size()) break;
        bool not_last = is_partitioned(elem_props.param_len, elem_index);

        // Safety: don't read past end of file
        if (elem_index + elem_props.param_len > _data.size()) break;

        interpret_elems(in_partition, elem_props, elem_index);
        in_partition = not_last;

    }

}



std::vector<ElementReader::CgmElement> ElementReader::elements() const {
    
    return _elements;

}


