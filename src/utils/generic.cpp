# include <fstream>
# include <iostream>

# include "utils/generic.hpp"



void generic_utils::log_unknown(const CgmElement& el, std::ofstream& log) {

    log << "[CGM] UNKNOWN  class="
            << static_cast<int>(el.elem_class)
            << "  id="
            << static_cast<int>(el.elem_id)
            << "  param_bytes="
            << el.params.size()
            << "\n";

}

