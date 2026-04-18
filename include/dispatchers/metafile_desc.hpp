# pragma once

# include <fstream>

# include "schemes/element.hpp"
# include "schemes/state.hpp"




class _MetafileDescriptor {

    const CgmElement& _elem;
    State& _state;
    std::ofstream& _log;

    public:
        _MetafileDescriptor(State& state, const CgmElement& elem, std::ofstream& log);
        void dispatch();

    private:
        void handle_vdc_type();
        void handle_integer_precision();
        void handle_real_precision();
        void handle_index_precision();
        // void handle_colour_precision(const CgmElement& elem);
        // void handle_colour_idx_prec(const CgmElement& elem);
        // void handle_name_precision(const CgmElement& elem);

};


