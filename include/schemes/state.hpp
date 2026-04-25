# pragma once

# include <cstdint>
# include <optional>
# include <variant>
# include <utility>
# include <array>
# include <map>
# include <memory>
# include <string>
# include <vector>
# include <unordered_map>

#include "schemes/element.hpp"
# include "schemes/flags.hpp"
# include "dom/node.hpp"





struct ColoursContext {


    struct RGB {

        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;

    };


    struct CMYK {

        std::uint8_t c = 0;
        std::uint8_t m = 0;
        std::uint8_t y = 0;
        std::uint8_t k = 0;

    };


    struct CIELAB {

        double L = 0.0;
        double a = 0.0;
        double b = 0.0;

    };


    struct CIELUV {

        double L = 0.0;
        double u = 0.0;
        double v = 0.0;

    };


    struct RGBRelated {

        double r = 0.0;
        double g = 0.0;
        double b = 0.0;

    };


    struct Value {

        std::variant<
            RGB, CMYK, CIELAB, CIELUV, RGBRelated
        > value = RGB{};

    };


    struct ScaleOffset {

        double scale  = 0.0;
        double offset = 0.0;
    };


    struct Extent {

        std::variant<

            std::pair<RGB,  RGB>,
            std::pair<CMYK, CMYK>,
            std::array<ScaleOffset, 3>
    
        > value = std::make_pair(

            RGB{0,   0,   0  },
            RGB{255, 255, 255}

        );

    };

};



struct TileArrayContext {

    double position_x = 0.0;
    double position_y = 0.0;
    int cell_path_direction = 0;   // 0=right 1=left 2=up 3=down
    int line_prog_direction = 0;   // 0=90CW  1=90CCW
    int num_tiles_path = 0;
    int num_tiles_line = 0;
    int num_cells_path_per_tile = 0;
    int num_cells_line_per_tile = 0;
    double cell_size_path = 0.0;
    double cell_size_line = 0.0;
    int image_offset_path = 0;
    int image_offset_line = 0;
    int image_num_cells_path = 0;
    int image_num_cells_line = 0;

};



// BEGIN/END APP STRUCTURE (0/21–23) — one entry per open APS level
struct APSEntryContext {

    std::string id;
    std::string type;
    int inheritance_flag = 0;  // 0=no-inherit  1=inherit
    std::map<std::string, std::string> attributes;  // populated by APS_ATTRIB (9/1)

};



struct SnapshotContext {

};



struct SegmentEntry {

    bool is_global = false;
    std::vector<std::unique_ptr<Node>> nodes;

};


struct CharSetEntry {

    int type = 0;
    std::string designation;

};


struct VdcExtent {
    double x1 = 0.0, y1 = 0.0;
    double x2 = 1.0, y2 = 1.0;
};



struct ColourCalibration {

    double Xn = 0.95047;
    double Yn = 1.00000;
    double Zn = 1.08830;

    // RGB matrix
    std::array<double, 9> matrix1{};

    // RGB-Related matrix
    std::array<double, 9> matrix2{};

    int lut_size = 0;
    struct LutPair {

        std::uint8_t in;
        std::uint8_t out;

    };

    std::vector<LutPair> lut_r;
    std::vector<LutPair> lut_g;
    std::vector<LutPair> lut_b;

    int grid_size = 0;
    std::vector<ColoursContext::Value> cmyk_grid_locations;

    struct XYZ { double X, Y, Z; };

    std::vector<XYZ> cmyk_grid_xyz;


};



struct State {

    Flags flags;
    using Colour = ColoursContext;

    int int_bytes = 2;
    int index_bytes = 2;
    int real_bytes = 4;
    int cidx_bytes = 1;
    int name_bytes = 2;
    int colour_bytes = 1;

    bool real_is_float = false;
    bool vdc_is_real = false;

    int colour_model = 1;
    int colour_sel_mode = 0;
    std::map<int, Colour::Value> colour_table;
    Colour::Value get_colour(int index) const;

    std::string metafile_name;
    std::optional<std::string> current_picture_name = std::nullopt;
    std::optional<std::uint32_t> current_segment_name = std::nullopt;

    void reset_picture_state();

    TileArrayContext tile_array_ctx;

    using APSEntry = APSEntryContext;
    std::vector<APSEntry> aps_stack; 
    std::map<std::string, APSEntry> aps_completed;

    using Snapshot = SnapshotContext;

    std::unordered_map<std::uint32_t, SegmentEntry> segment_store;
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>> protection_regions;


    // class 1
    int metafile_version = 0;
    std::string metafile_desc;

    int max_colour_index = 63;
    Colour::Extent colour_value_extent;

    std::vector<CgmElement> metafile_defaults;
    std::vector<std::string> font_list;
    std::vector<CharSetEntry> char_set_list; 

    int char_coding_announcer = 0;

    VdcExtent max_vdc_extent;

    int segment_priority_min = 0;
    int segment_priority_max = 0;

    ColourCalibration colour_calibration;

};


