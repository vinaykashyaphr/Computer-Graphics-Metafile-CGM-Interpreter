# pragma once

# include <fstream>
# include <unordered_map>
# include <memory>

# include "schemes/element.hpp"
# include "schemes/graphics_state.hpp"
# include "dom/node.hpp"



class _Delimiter {

    const CgmElement& _elem;
    GraphicsState& _state;
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>>& _segment_store;
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>>& _protection_regions;
    std::ofstream& _log;


    public:
        _Delimiter(
            GraphicsState& state,
            const CgmElement& elem,
            std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>>& segment_store,
            std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>>& protection_regions,
            std::ofstream& log
        );
        void dispatch();

    private:
        void _begin_metafile();
        void _end_metafile();
        void _begin_picture();
        void _begin_picture_body();
        void _end_picture();
        void _begin_segment();
        void _end_segment();
        void _begin_figure();
        void _end_figure();
        void _begin_protection_region();
        void _end_protection_region();
        void _begin_compound_line();
        void _end_compound_line();
        void _begin_compound_text_path();
        void _end_compound_text_path();
        void _begin_tile_array();
        void _end_tile_array();
        void _begin_app_structure();
        void _begin_app_structure_body();
        void _end_app_structure();

};


