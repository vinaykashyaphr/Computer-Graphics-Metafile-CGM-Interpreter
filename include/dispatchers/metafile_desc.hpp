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
        void _metafile_version();
        void _metafile_desc();
        void _vdc_type();
        void _integer_precision();
        void _real_precision();
        void _index_precision();
        void _colour_precision();
        void _colour_index_precision();
        void _max_colour_index();
        void _colour_value_extent();
        void _metafile_element_list();
        void _metafile_defaults_replacement();
        void _font_list();
        void _character_set_list();
        void _character_coding_announcer();

        // void handle_colour_idx_prec(const CgmElement& elem);
        // void handle_name_precision(const CgmElement& elem);

};


