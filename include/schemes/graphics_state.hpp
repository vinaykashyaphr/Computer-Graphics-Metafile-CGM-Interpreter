# pragma once

# include <cstdint>
# include <map>
# include <string>
# include <vector>
# include <array>
# include <variant>


struct GraphicsState {

    struct ColourRGB {

        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;

    };

    struct ColourCMYK {

        std::uint8_t c = 0;
        std::uint8_t m = 0;
        std::uint8_t y = 0;
        std::uint8_t k = 0;

    };

    struct ColourCIELAB {

        double L = 0.0;
        double a = 0.0;
        double b = 0.0;

    };

    struct ColourCIELUV {

        double L = 0.0;
        double u = 0.0;
        double v = 0.0;

    };

    struct ColourRGBRelated {

        double r = 0.0;
        double g = 0.0;
        double b = 0.0;

    };

    struct ColourValue {

        std::variant<
            ColourRGB, ColourCMYK, ColourCIELAB, ColourCIELUV, ColourRGBRelated
        > value = ColourRGB{};

    };

    struct ColourScaleOffset {

        double scale  = 0.0;
        double offset = 0.0;
    };

    struct ColourExtent {

        std::variant<

            std::pair<ColourRGB,  ColourRGB>,
            std::pair<ColourCMYK, ColourCMYK>,
            std::array<ColourScaleOffset, 3>
    
        > value = std::make_pair(

            ColourRGB{0,   0,   0  },
            ColourRGB{255, 255, 255}

        );

    };

    int version = 1;
    bool vdc_is_real = false;
    bool real_is_float = false;
    int int_bytes = 2;
    int real_bytes = 4;
    int index_bytes = 2;
    int cidx_bytes = 1;
    int colour_bytes = 1;
    int name_bytes = 2;
    int max_colour_index = 63;

    int colour_model = 1;
    ColourExtent colour_value_extent;

    std::string metafile_name;
    std::string metafile_desc;

    int seg_priority_min = 0;
    int seg_priority_max = 255;

    std::vector<std::string> font_list;

    std::vector<std::string> char_set_list;
    int char_coding = 0;

    std::vector<std::string> symbol_lib_list;

    double vdc_x1 = 0.0;
    double vdc_y1 = 0.0;
    double vdc_x2 = 32767.0;
    double vdc_y2 = 32767.0;

    int scaling_mode = 0;
    double scale_factor = 1.0;

    int colour_sel_mode = 0;
    int line_width_mode = 1;
    int marker_size_mode = 1;
    int edge_width_mode = 1;
    int interior_style_mode = 0;

    ColourRGB background_colour = {255, 255, 255};

    double viewport_x1 = 0.0;
    double viewport_y1 = 0.0;
    double viewport_x2 = 1.0;
    double viewport_y2 = 1.0;

    int viewport_spec_mode = 0;
    double viewport_scale = 1.0;

    bool viewport_isotropic = true;
    int viewport_h_align = 0;
    int viewport_v_align = 0;

    struct LineBundle {

        int line_type = 1;
        double line_width = 1.0;
        int line_colour = 1;

    };

    struct MarkerBundle {

        int marker_type = 3;
        double marker_size = 1.0;
        int marker_colour = 1;

    };

    struct TextBundle {

        int font_index = 1;
        int text_precision = 0;
        double char_expansion = 1.0;
        double char_spacing = 0.0;
        int text_colour = 1;

    };

    struct FillBundle {

        // hollow
        int interior_style = 0;

        int fill_colour = 1;
        int hatch_index = 1;
        int pattern_index = 1;

    };

    struct EdgeBundle {

        int    edge_type = 1;
        double edge_width = 1.0;
        int    edge_colour = 1;

    };

    std::map<int, LineBundle>   line_bundles;
    std::map<int, MarkerBundle> marker_bundles;
    std::map<int, TextBundle>   text_bundles;
    std::map<int, FillBundle>   fill_bundles;
    std::map<int, EdgeBundle>   edge_bundles;

    struct DashPattern {

        int line_type_index = -1;
        double cycle_length = 0.0;
        std::vector<int> dash_elements;

    };

    struct HatchStyle {

        int    hatch_index = -1;
        int    style = 0;
        double gap_width = 0.0;

    };

    std::map<int, DashPattern> custom_dash_patterns;
    std::map<int, HatchStyle>  custom_hatch_styles;

    // GEOMETRIC PATTERN DEFINITIONS (2/19)
    struct GeometricPattern {

        int    segment_name = 0;
        double x1 = 0.0;
        double y1 = 0.0;
        double x2 = 0.0;
        double y2 = 0.0;

    };

    std::map<int, GeometricPattern> geometric_patterns;

    // PATTERN TABLE (5/32) — stored here, copied into Snapshot
    struct PatternEntry {

        int nx = 0;
        int ny = 0;
        std::vector<ColourValue> cells;

    };

    std::map<int, PatternEntry> pattern_table;


    // ═══════════════════════════════════════════════════════════
    // CONTROL  (class 3)
    // [GS] only — except mitre_limit and transparent_cell which ARE in Snapshot
    // ═══════════════════════════════════════════════════════════

    // CLIP INDICATOR (3/6) + CLIP RECTANGLE (3/5)
    // ISO 8632-1 cl.8: clip indicator default=on. clip rectangle default=VDC EXTENT
    bool   clip_active = true;    // default=on per spec
    double clip_x1 = 0.0;
    double clip_y1 = 0.0;
    double clip_x2 = 32767.0;    // default=VDC EXTENT default
    double clip_y2 = 32767.0;

    // CLIPPING MODES (3/7, 3/8, 3/9)
    int line_clip_mode = 0;    // 0=locus 1=shape 2=locus+shape
    int marker_clip_mode = 0;
    int edge_clip_mode = 0;

    // TRANSPARENCY (3/4)
    // ISO 8632-1 cl.8: default=on (1)
    int transparency = 1;  // 1=on (transparent background)

    // AUXILIARY COLOUR (3/3)
    // ISO 8632-1 cl.8: default=0 if indexed
    ColourValue aux_colour;  // default ColourRGB{0,0,0} via variant default

    // GENERALIZED TEXT PATH MODE (3/18)
    int gen_text_path_mode = 0;  // 0=off. ISO 8632-1 cl.8 default=off

    // PROTECTION REGION INDICATOR (3/17)
    int protection_region_index = 1;  // ISO 8632-1 cl.8: default region=1
    int protection_region_state = 0;  // 0=off


    // ═══════════════════════════════════════════════════════════
    // SEGMENT STATE  (class 8)
    // [GS] only
    // ═══════════════════════════════════════════════════════════

    struct SegmentTransform {

        // 2×2 scale/rotate matrix
        double a11 = 1.0;
        double a12 = 0.0;
        double a21 = 0.0;
        double a22 = 1.0;

        // translation vector
        double a13 = 0.0;
        double a23 = 0.0;

    };

    std::map<int, SegmentTransform> segment_transforms;
    std::map<int, bool>             segment_highlighting;
    std::map<int, int>              segment_display_priority;
    std::map<int, int>              segment_pick_priority;

    // INHERITANCE FILTER (8/2)
    // ISO 8632-1 cl.8: default=segment for all attributes
    int inheritance_filter = 1;  // 0=state list 1=segment

    // CLIP INHERITANCE (8/3)
    // ISO 8632-1 cl.8: default=state list
    int clip_inheritance = 0;  // 0=state list 1=intersection


    // ═══════════════════════════════════════════════════════════
    // DRAWING STYLE  (class 5 attribute elements)
    // [GS+SNAP] — ALL of these go into Snapshot
    // defaults from ISO 8632-1 clause 8
    // ═══════════════════════════════════════════════════════════

    // bundle indexes
    int line_bundle_index = 1;    // LINE BUNDLE INDEX (5/1). default=1
    int marker_bundle_index = 1;  // MARKER BUNDLE INDEX (5/5). default=1
    int text_bundle_index = 1;    // TEXT BUNDLE INDEX (5/9). default=1
    int fill_bundle_index = 1;    // FILL BUNDLE INDEX (5/21). default=1
    int edge_bundle_index = 1;    // EDGE BUNDLE INDEX (5/26). default=1

    // ASPECT SOURCE FLAGS (5/35): 0=individual, 1=bundled
    // ISO 8632-1 cl.8: default=all individual (0)
    std::array<int, 18> aspect_source_flags = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

    // LINE (5/2, 5/3, 5/4, 5/37, 5/38, 5/39, 5/40)
    int    line_type = 1;           // 1=solid. ISO 8632-1 cl.8 default=1
    double line_width = 1.0;        // scaled mode default=1.0
    int    line_colour = 1;         // indexed default=1
    int    line_cap = 1;            // 1=unspecified. ISO 8632-1 cl.8 default=1
    int    line_join = 1;           // 1=unspecified. ISO 8632-1 cl.8 default=1
    int    line_type_cont = 1;      // 1=unspecified. ISO 8632-1 cl.8 default=1
    double line_type_offset = 0.0;  // ISO 8632-1 cl.8 default=0.0

    // MARKER (5/6, 5/7, 5/8)
    int    marker_type = 3;    // 3=asterisk. ISO 8632-1 cl.8 default=3
    double marker_size = 1.0;  // scaled mode default=1.0
    int    marker_colour = 1;  // indexed default=1

    // FILL (5/22, 5/23, 5/24, 5/25, 5/31, 5/32, 5/33)
    int    interior_style = 0;      // 0=hollow. ISO 8632-1 cl.8 default=hollow
    int    fill_colour = 1;         // ISO 8632-1 cl.8 default=1 (indexed)
    int    hatch_index = 1;         // ISO 8632-1 cl.8 default=1
    int    pattern_index = 1;       // ISO 8632-1 cl.8 default=1
    double fill_ref_x = 0.0;        // FILL REFERENCE POINT (5/31). default=lower-left of VDC EXTENT
    double fill_ref_y = 0.0;
    // PATTERN SIZE (5/33): default=0,dy,dx,0 where dy/dx are VDC extent dimensions
    double pattern_size_hx = 0.0;
    double pattern_size_hy = 32767.0;  // dy = default VDC height
    double pattern_size_wx = 32767.0;  // dx = default VDC width
    double pattern_size_wy = 0.0;

    // EDGE (5/27, 5/28, 5/29, 5/30, 5/44, 5/45, 5/46, 5/47)
    bool   edge_visible = false;     // EDGE VISIBILITY (5/30). ISO 8632-1 cl.8 default=off
    int    edge_type = 1;            // 1=solid. default=1
    double edge_width = 1.0;         // scaled mode default=1.0
    int    edge_colour = 1;          // indexed default=1
    int    edge_cap = 1;             // 1=unspecified. ISO 8632-1 cl.8 default=1
    int    edge_join = 1;            // 1=unspecified. ISO 8632-1 cl.8 default=1
    int    edge_type_cont = 1;       // 1=unspecified. ISO 8632-1 cl.8 default=1
    double edge_type_offset = 0.0;   // ISO 8632-1 cl.8 default=0.0

    // TEXT (5/10–5/20, 5/41, 5/42)
    int    text_colour = 1;          // indexed default=1
    // CHARACTER HEIGHT (5/15): default=1/100 of longest VDC side
    // For default integer VDC (32767): 32767/100 = 327.67
    double char_height = 327.67;
    int    font_index = 1;           // TEXT FONT INDEX (5/10). default=1
    int    text_precision = 0;       // 0=string. ISO 8632-1 cl.8 default=string
    int    text_path = 0;            // 0=right. ISO 8632-1 cl.8 default=right
    int    text_align_h = 0;         // 0=normal horizontal
    int    text_align_v = 0;         // 0=normal vertical
    double text_align_ch = 0.0;
    double text_align_cv = 0.0;
    double char_spacing = 0.0;       // ISO 8632-1 cl.8 default=0.0
    double char_expansion = 1.0;     // ISO 8632-1 cl.8 default=1.0
    // CHARACTER ORIENTATION (5/16): default=(0,dy,dy,0) where dy=VDC height
    // For default integer VDC: dy=32767
    double char_up_x = 0.0;
    double char_up_y = 32767.0;      // ISO 8632-1 cl.8: up vector = (0, dy)
    double char_base_x = 32767.0;   // ISO 8632-1 cl.8: base vector = (dy, 0)
    double char_base_y = 0.0;
    int    char_set_index = 1;       // ISO 8632-1 cl.8 default=1
    int    alt_char_set_index = 1;   // ISO 8632-1 cl.8 default=1
    int    restricted_text_type = 1; // 1=basic. ISO 8632-1 cl.8 default=1
    int    text_score_type = 0;      // all off. ISO 8632-1 cl.8 default=all off

    int symbol_lib_index = 1;
    int symbol_colour = 1;
    double symbol_size_h = 327.67;
    double symbol_size_w = 327.67;
    int symbol_size_mode = 0;
    double symbol_up_x = 0.0;
    double symbol_up_y = 32767.0;
    double symbol_base_x = 32767.0;
    double symbol_base_y = 0.0;

    int pick_identifier = 0;
    double mitre_limit = 32767.0;

    int transparent_cell_on = 0;
    ColourValue transparent_cell_colour;

    struct InterpolatedInterior {

        int style = 1;
        std::vector<double> ref_geometry;
        std::vector<double> stage_designators;
        std::vector<ColourValue> colours;

    };

    InterpolatedInterior interpolated_interior;
    std::map<int, ColourValue> colour_table;


    struct Snapshot {

        // bundle indexes
        int line_bundle_index = 1;
        int marker_bundle_index = 1;
        int text_bundle_index = 1;
        int fill_bundle_index = 1;
        int edge_bundle_index = 1;

        std::array<int, 18> aspect_source_flags = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };

        // line
        int    line_type = 1;
        double line_width = 1.0;
        int    line_colour = 1;
        int    line_cap = 1;
        int    line_join = 1;
        int    line_type_cont = 1;
        double line_type_offset = 0.0;

        // marker
        int    marker_type = 3;
        double marker_size = 1.0;
        int    marker_colour = 1;

        // fill
        int    interior_style = 0;
        int    fill_colour = 1;
        int    hatch_index = 1;
        int    pattern_index = 1;
        double fill_ref_x = 0.0;
        double fill_ref_y = 0.0;
        double pattern_size_hx = 0.0;
        double pattern_size_hy = 32767.0;
        double pattern_size_wx = 32767.0;
        double pattern_size_wy = 0.0;

        // edge
        bool   edge_visible = false;
        int    edge_type = 1;
        double edge_width = 1.0;
        int    edge_colour = 1;
        int    edge_cap = 1;
        int    edge_join = 1;
        int    edge_type_cont = 1;
        double edge_type_offset = 0.0;

        // text
        int    text_colour = 1;
        double char_height = 327.67;
        int    font_index = 1;
        int    text_precision = 0;
        int    text_path = 0;
        int    text_align_h = 0;
        int    text_align_v = 0;
        double text_align_ch = 0.0;
        double text_align_cv = 0.0;
        double char_spacing = 0.0;
        double char_expansion = 1.0;
        double char_up_x = 0.0;
        double char_up_y = 32767.0;
        double char_base_x = 32767.0;
        double char_base_y = 0.0;
        int    char_set_index = 1;
        int    alt_char_set_index = 1;
        int    restricted_text_type = 1;
        int    text_score_type = 0;

        // symbol
        int    symbol_lib_index = 1;
        int    symbol_colour = 1;
        double symbol_size_h = 327.67;
        double symbol_size_w = 327.67;
        int    symbol_size_mode = 0;
        double symbol_up_x = 0.0;
        double symbol_up_y = 32767.0;
        double symbol_base_x = 32767.0;
        double symbol_base_y = 0.0;

        // pick
        int pick_identifier = 0;

        int transparent_cell_on = 0;
        double mitre_limit = 32767.0;

        ColourValue transparent_cell_colour;
        InterpolatedInterior interpolated_interior;

        std::map<int, ColourValue> colour_table;

        std::map<int, LineBundle> line_bundles;
        std::map<int, MarkerBundle> marker_bundles;
        std::map<int, TextBundle> text_bundles;
        std::map<int, FillBundle> fill_bundles;
        std::map<int, EdgeBundle> edge_bundles;

        std::map<int, PatternEntry> pattern_table;
        std::map<int, DashPattern> custom_dash_patterns;
        std::map<int, HatchStyle>  custom_hatch_styles;

    };


    Snapshot snapshot() const {

        return {

            line_bundle_index,
            marker_bundle_index,
            text_bundle_index,
            fill_bundle_index,
            edge_bundle_index,

            aspect_source_flags,

            line_type,
            line_width,
            line_colour,
            line_cap,
            line_join,
    
            line_type_cont,
            line_type_offset,

            marker_type,
            marker_size,
            marker_colour,

            interior_style,
            fill_colour,
            hatch_index,
            pattern_index,

            fill_ref_x,
            fill_ref_y,

            pattern_size_hx,
            pattern_size_hy,
            pattern_size_wx,
            pattern_size_wy,

            edge_visible,
            edge_type,
            edge_width,
            edge_colour,

            edge_cap,
            edge_join,
            edge_type_cont,
            edge_type_offset,

            text_colour,
            char_height,
            font_index,
            text_precision,
            text_path,

            text_align_h,
            text_align_v,
            text_align_ch,
            text_align_cv,
    
            char_spacing,
            char_expansion,
    
            char_up_x,
            char_up_y,
            char_base_x,
            char_base_y,

            char_set_index,
            alt_char_set_index,

            restricted_text_type,
            text_score_type,

            symbol_lib_index,
            symbol_colour,
            symbol_size_h,
            symbol_size_w,
            symbol_size_mode,

            symbol_up_x,
            symbol_up_y,
            symbol_base_x,
            symbol_base_y,

            pick_identifier,

            transparent_cell_on,
            mitre_limit,
            transparent_cell_colour,
            interpolated_interior,

            colour_table,

            line_bundles,
            marker_bundles,
            text_bundles,
            fill_bundles,
            edge_bundles,

            pattern_table,

            custom_dash_patterns,
            custom_hatch_styles

        };

    }

    // SAVE/RESTORE PRIMITIVE CONTEXT (3/11, 3/12)
    // stack of snapshots keyed by name integer
    std::map<int, struct Snapshot> context_stack;


    void reset_picture_state() {

        vdc_x1 = 0.0;
        vdc_y1 = 0.0;

        vdc_x2 = 32767.0;
        vdc_y2 = 32767.0;

        scaling_mode = 0;
        scale_factor = 1.0;

        colour_sel_mode = 0;
        line_width_mode = 1;
        marker_size_mode = 1;
        edge_width_mode = 1;
        interior_style_mode = 0;

        background_colour = {255, 255, 255};

        viewport_x1 = 0.0;
        viewport_y1 = 0.0;

        viewport_x2 = 1.0;
        viewport_y2 = 1.0;

        viewport_spec_mode = 0;
        viewport_scale = 1.0;

        viewport_isotropic = true;
        viewport_h_align = 0;
        viewport_v_align = 0;

        line_bundles.clear();
        marker_bundles.clear();
        text_bundles.clear();

        fill_bundles.clear();
        edge_bundles.clear();

        custom_dash_patterns.clear();
        custom_hatch_styles.clear();

        geometric_patterns.clear();
        pattern_table.clear();

        clip_active = true;

        clip_x1 = 0.0;
        clip_y1 = 0.0;
        clip_x2 = 32767.0;
        clip_y2 = 32767.0;

        line_clip_mode = 0;
        marker_clip_mode = 0;
        edge_clip_mode = 0;

        transparency = 1;

        aux_colour = ColourValue{ColourRGB{0, 0, 0}};
        gen_text_path_mode = 0;
        protection_region_index = 1;
        protection_region_state = 0;

        context_stack.clear();

        segment_transforms.clear();
        segment_highlighting.clear();
        segment_display_priority.clear();
        segment_pick_priority.clear();

        inheritance_filter = 1;
        clip_inheritance = 0;


        line_bundle_index = 1;
        marker_bundle_index = 1;
        text_bundle_index = 1;
        fill_bundle_index = 1;
        edge_bundle_index = 1;

        aspect_source_flags.fill(0);

        line_type = 1;
        line_width = 1.0;
        line_colour = 1;
        line_cap = 1;
        line_join = 1;
        line_type_cont = 1;
        line_type_offset = 0.0;

        marker_type = 3;
        marker_size = 1.0;
        marker_colour = 1;

        interior_style = 0;
        fill_colour = 1;
        hatch_index = 1;
        pattern_index = 1;

        fill_ref_x = 0.0;
        fill_ref_y = 0.0;

        pattern_size_hx = 0.0;
        pattern_size_hy = 32767.0;
        pattern_size_wx = 32767.0;
        pattern_size_wy = 0.0;

        edge_visible = false;
        edge_type = 1;
        edge_width = 1.0;
        edge_colour = 1;

        edge_cap = 1;
        edge_join = 1;
        edge_type_cont = 1;
        edge_type_offset = 0.0;

        text_colour = 1;
        char_height = 327.67;
        font_index = 1;

        text_precision = 0;
        text_path = 0;

        text_align_h = 0;
        text_align_v = 0;
        text_align_ch = 0.0;
        text_align_cv = 0.0;

        char_spacing = 0.0;
        char_expansion = 1.0;

        char_up_x = 0.0;
        char_up_y = 32767.0;

        char_base_x = 32767.0;
        char_base_y = 0.0;
        char_set_index = 1; alt_char_set_index = 1;
        restricted_text_type = 1; text_score_type = 0;

        symbol_lib_index = 1;
        symbol_colour = 1;

        symbol_size_h = 327.67;
        symbol_size_w = 327.67;
        symbol_size_mode = 0;

        symbol_up_x = 0.0;
        symbol_up_y = 32767.0;
        symbol_base_x = 32767.0;
        symbol_base_y = 0.0;

        pick_identifier = 0;
        transparent_cell_on = 0;
        mitre_limit = 32767.0;

        transparent_cell_colour = ColourValue{ColourRGB{0, 0, 0}};
        interpolated_interior = InterpolatedInterior{};

        colour_table.clear();

    }

};
