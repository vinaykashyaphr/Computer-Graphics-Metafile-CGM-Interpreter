#pragma once

#include <cstdint>




// LEGEND:
// [GS]       → updates GraphicsState only (precision, canvas, structural)
// [GS+SNAP]  → updates GraphicsState AND must be in Snapshot (visual style)
// [DOM]      → creates a node in the DOM tree
// [LOG]      → log it, no handler needed yet — but never silently discard
// NOTE: nothing is silently ignored. every element gets at minimum logged.

enum class CgmClass : std::uint8_t {

    DELIMITER            = 0,
    METAFILE_DESCRIPTOR  = 1,
    PICTURE_DESCRIPTOR   = 2,
    CONTROL              = 3,
    GEOMETRY             = 4,
    ATTRIBUTE            = 5,
    ESCAPE               = 6,
    EXTERNAL             = 7,
    SEGMENT              = 8,
    APS                  = 9

};



enum class Delimiter : std::uint8_t {

    NO_OP                    = 0,   // [GS] skip param bytes, advance i, done

    /// @p metafile_name [GS]
    BEGIN_METAFILE           = 1,

    END_METAFILE             = 2,   // [GS] stop parsing
    BEGIN_PICTURE            = 3,   // [GS] read picture name, reset state
    BEGIN_PICTURE_BODY       = 4,   // [GS] geometry stream starts
    END_PICTURE              = 5,   // [GS] geometry stream ends
    BEGIN_SEGMENT            = 6,   // [GS] open named reusable segment
    END_SEGMENT              = 7,   // [GS] close segment
    BEGIN_FIGURE             = 8,   // [GS] open figure group
    END_FIGURE               = 9,   // [GS] close figure group
    BEGIN_PROTECTION_REGION  = 13,  // [GS] open protected region
    END_PROTECTION_REGION    = 14,  // [GS] close protected region
    BEGIN_COMPOUND_LINE      = 15,  // [GS] open compound line group
    END_COMPOUND_LINE        = 16,  // [GS] close compound line group
    BEGIN_COMPOUND_TEXT_PATH = 17,  // [GS] open compound text path
    END_COMPOUND_TEXT_PATH   = 18,  // [GS] close compound text path
    BEGIN_TILE_ARRAY         = 19,  // [GS] open tile array group
    END_TILE_ARRAY           = 20,  // [GS] close tile array group
    BEGIN_APP_STRUCTURE      = 21,  // [GS] push APS node onto stack
    BEGIN_APP_STRUCTURE_BODY = 22,  // [GS] geometry inside APS starts
    END_APP_STRUCTURE        = 23   // [GS] pop APS stack

};



enum class MetafileDescriptor : std::uint8_t {

    METAFILE_VERSION         = 1,   // [GS] _state.version

    /// @p metafile_desc [GS]
    METAFILE_DESCRIPTION     = 2,   // [GS] read profile string

    /// @p vdc_is_real [GS]
    VDC_TYPE                 = 3,

    /// @p int_bytes [GS]
    INTEGER_PRECISION        = 4,

    /// @p real_bytes;real_is_float [GS]
    REAL_PRECISION           = 5,

    /// @p index_bytes [GS]
    INDEX_PRECISION          = 6,

    /// @p colour_bytes [GS]
    COLOUR_PRECISION         = 7,

    /// @p cidx_bytes [GS]
    COLOUR_INDEX_PRECISION   = 8,

    /// @p max_colour_index [GS]
    MAX_COLOUR_INDEX         = 9,

    /// @p colour_extent_cie;ColourScaleOffset;scale;offset;colour_rgb_min;colour_rgb_max [GS]
    COLOUR_VALUE_EXTENT      = 10,  // [GS] _state.colour_min/max range
    METAFILE_ELEMENT_LIST    = 11,  // [GS] list of elements present in file
    METAFILE_DEFAULTS_REPL   = 12,  // [GS] overrides precision defaults

    /// @p font_list [GS]
    FONT_LIST                = 13,

    /// @p char_set_list [GS]
    CHAR_SET_LIST            = 14,

    /// @p char_coding [GS]
    CHAR_CODING_ANNOUNCER    = 15,

    /// @p name_bytes [GS]
    NAME_PRECISION           = 16,

    MAX_VDC_EXTENT           = 17,  // [GS] largest coordinate values

    /// @p segment_priority_min;segment_priority_max [GS]
    SEGMENT_PRIORITY_EXTENT  = 18,

    /// @p colour_model;RGB;CIELAB;CIELUV;RGBRelated;CMYK [GS]
    COLOUR_MODEL              = 19,

    COLOUR_CALIBRATION        = 20,  // [GS] _state.colour_calibration
    FONT_PROPERTIES          = 21,  // [GS] _state.font_properties list
    GLYPH_MAPPING            = 22,  // [GS] _state.glyph_mapping list
    SYMBOL_LIB_LIST          = 23,  // [GS] _state.symbol_lib_list
    PICTURE_DIR              = 24   // [GS] picture location index

};



enum class PictureDescriptor : std::uint8_t {

    /// @p scalling_mode;scale_factor [GS]
    SCALING_MODE             = 1,

    /// @p colour_sel_mode [GS]
    COLOUR_SELECTION_MODE    = 2,

    /// @p line_width_mode [GS]
    LINE_WIDTH_SPEC_MODE     = 3,

    /// @p marker_size_mode [GS]
    MARKER_SIZE_SPEC_MODE    = 4,

    /// @p edge_width_mode [GS]
    EDGE_WIDTH_SPEC_MODE     = 5,

    /// @p vdc_x1;vdc_x2;vdc_y1;vdc_y2 [GS]
    VDC_EXTENT               = 6,

    /// @p background_colour [GS]
    BACKGROUND_COLOUR        = 7,

    /// @p viewport_x1;viewport_x2;viewport_y1;viewport_y2 [GS]
    DEVICE_VIEWPORT          = 8,

    /// @p viewport_spec_mode;viewport_scale [GS]
    DEVICE_VIEWPORT_SPEC_MODE= 9,

    /// @p viewport_isotropic;viewport_h_align;viewport_y_align
    DEVICE_VIEWPORT_MAPPING  = 10,  // [GS] _state.viewport_mapping
    LINE_REPR                = 11,  // [GS] line bundle table entry
    MARKER_REPR              = 12,  // [GS] marker bundle table entry
    TEXT_REPR                = 13,  // [GS] text bundle table entry
    FILL_REPR                = 14,  // [GS] fill bundle table entry
    EDGE_REPR                = 15,  // [GS] edge bundle table entry

    /// @p interior_style_mode [GS]
    INTERIOR_STYLE_SPEC_MODE = 16,

    LINE_AND_EDGE_TYPE_DEF   = 17,  // [GS] _state.custom_dash_patterns
    HATCH_STYLE_DEF          = 18,  // [GS] _state.custom_hatch_styles
    GEOMETRIC_PATTERN_DEF    = 19,  // [GS] _state.geometric_patterns
    APS_DIR                  = 20   // [GS] APS location directory

};



enum class Control : std::uint8_t {

    VDC_INTEGER_PRECISION       = 1,   // [GS] override int precision mid-picture
    VDC_REAL_PRECISION          = 2,   // [GS] override real precision mid-picture

    /// @p aux_colour [GS]
    AUX_COLOUR                  = 3,

    TRANSPARENCY                = 4,   // [GS] _state.transparency
    CLIP_RECTANGLE              = 5,   // [GS] _state.clip_x1/y1/x2/y2
    CLIP_INDICATOR              = 6,   // [GS] _state.clip_active
    LINE_CLIPPING_MODE          = 7,   // [GS] _state.line_clip_mode
    MARKER_CLIPPING_MODE        = 8,   // [GS] _state.marker_clip_mode
    EDGE_CLIPPING_MODE          = 9,   // [GS] _state.edge_clip_mode
    NEW_REGION                  = 10,  // [GS] clear drawing region
    SAVE_PRIMITIVE_CONTEXT      = 11,  // [GS] push graphics state onto stack
    RESTORE_PRIMITIVE_CONTEXT   = 12,  // [GS] pop graphics state from stack
    PROTECTION_REGION_INDICATOR = 17,  // [GS] _state.protection_region
    GEN_TEXT_PATH_MODE          = 18,  // [GS] _state.gen_text_path_mode
    MITRE_LIMIT                 = 19,  // [GS+SNAP] _state.mitre_limit
    TRANSPARENCY_CELL_COLOUR     = 20   // [GS+SNAP] _state.transparent_cell_colour

};



enum class Geometry : std::uint8_t {

    POLYLINE                    = 1,   // [DOM] → PolylineNode
    DISJOINT_POLYLINE           = 2,   // [DOM] → DisjointPolylineNode
    POLYMARKER                  = 3,   // [DOM] → PolymarkerNode
    TEXT                        = 4,   // [DOM] → TextNode
    RESTRICTED_TEXT             = 5,   // [DOM] → TextNode (most common)
    APPEND_TEXT                 = 6,   // [DOM] → appends string to last TextNode
    POLYGON                     = 7,   // [DOM] → PolygonNode
    POLYGON_SET                 = 8,   // [DOM] → PolygonSetNode
    CELL_ARRAY                  = 9,   // [DOM] → CellArrayNode (raster image)
    GEN_DWG_PRIMITIVE           = 10,  // [DOM] → GdpNode (vendor primitive)
    RECTANGLE                   = 11,  // [DOM] → RectNode
    CIRCLE                      = 12,  // [DOM] → CircleNode
    CIRCULAR_ARC_3PT            = 13,  // [DOM] → ArcNode (3 point form)
    CIRCULAR_ARC_3PT_CLOSE      = 14,  // [DOM] → ArcNode (3 point, closed)
    CIRCULAR_ARC_CENTRE         = 15,  // [DOM] → ArcNode (centre form)
    CIRCULAR_ARC_CENTRE_CLOSE   = 16,  // [DOM] → ArcNode (centre, closed)
    ELLIPSE                     = 17,  // [DOM] → EllipseNode
    ELLIPTICAL_ARC              = 18,  // [DOM] → EllipticArcNode
    ELLIPTICAL_ARC_CLOSE        = 19,  // [DOM] → EllipticArcNode (closed)
    CIRC_ARC_CENTRE_REVERSED    = 20,  // [DOM] → ArcNode (reverse winding)
    CONNECTING_EDGE             = 21,  // [DOM] → ConnectingEdgeNode
    HYPERBOLIC_ARC              = 22,  // [DOM] → ConicArcNode (hyperbola)
    PARABOLIC_ARC               = 23,  // [DOM] → ConicArcNode (parabola)
    NON_UNIFORM_B_SPLINE        = 24,  // [DOM] → SplineNode
    NURBS                       = 25,  // [DOM] → NurbsNode
    POLYBEZIER                  = 26,  // [DOM] → BezierNode
    POLYSYMBOL                  = 27,  // [DOM] → PolysymbolNode
    BITONAL_TILE                = 28,  // [DOM] → TileNode (1-bit raster)
    TILE                        = 29   // [DOM] → TileNode (colour raster)

};



enum class Attribute : std::uint8_t {

    LINE_BUNDLE_INDEX        = 1,   // [GS] _state.line_bundle_index
    LINE_TYPE                = 2,   // [GS+SNAP] _state.line_type
    LINE_WIDTH               = 3,   // [GS+SNAP] _state.line_width
    LINE_COLOUR              = 4,   // [GS+SNAP] _state.line_colour
    MARKER_BUNDLE_INDEX      = 5,   // [GS] _state.marker_bundle_index
    MARKER_TYPE              = 6,   // [GS+SNAP] _state.marker_type
    MARKER_SIZE              = 7,   // [GS+SNAP] _state.marker_size
    MARKER_COLOUR            = 8,   // [GS+SNAP] _state.marker_colour
    TEXT_BUNDLE_INDEX        = 9,   // [GS] _state.text_bundle_index
    TEXT_FONT_INDEX          = 10,  // [GS+SNAP] _state.font_index
    TEXT_PRECISION           = 11,  // [GS+SNAP] _state.text_precision
    CHAR_EXPANSION_FACTOR    = 12,  // [GS+SNAP] _state.char_expansion
    CHAR_SPACING             = 13,  // [GS+SNAP] _state.char_spacing
    TEXT_COLOUR              = 14,  // [GS+SNAP] _state.text_colour
    CHAR_HEIGHT              = 15,  // [GS+SNAP] _state.char_height
    CHAR_ORIENTATION         = 16,  // [GS+SNAP] _state.char_up/base x/y
    TEXT_PATH                = 17,  // [GS+SNAP] _state.text_path
    TEXT_ALIGNMENT           = 18,  // [GS+SNAP] _state.text_align_h/v
    CHAR_SET_INDEX           = 19,  // [GS+SNAP] _state.char_set_index
    ALT_CHAR_SET_INDEX       = 20,  // [GS+SNAP] _state.alt_char_set_index
    FILL_BUNDLE_INDEX        = 21,  // [GS] _state.fill_bundle_index
    INTERIOR_STYLE           = 22,  // [GS+SNAP] _state.interior_style
    FILL_COLOUR              = 23,  // [GS+SNAP] _state.fill_colour
    HATCH_INDEX              = 24,  // [GS+SNAP] _state.hatch_index
    PATTERN_INDEX            = 25,  // [GS+SNAP] _state.pattern_index
    EDGE_BUNDLE_INDEX        = 26,  // [GS] _state.edge_bundle_index
    EDGE_TYPE                = 27,  // [GS+SNAP] _state.edge_type
    EDGE_WIDTH               = 28,  // [GS+SNAP] _state.edge_width
    EDGE_COLOUR              = 29,  // [GS+SNAP] _state.edge_colour
    EDGE_VISIBILITY          = 30,  // [GS+SNAP] _state.edge_visible
    FILL_REF_POINT           = 31,  // [GS+SNAP] _state.fill_ref_x/y
    PATTERN_TABLE            = 32,  // [GS] _state.pattern_table
    PATTERN_SIZE             = 33,  // [GS+SNAP] _state.pattern_size

    /// @p colour_table;Colour [GS]
    COLOUR_TABLE             = 34,

    ASPECT_SOURCE_FLAGS      = 35,  // [GS] _state.aspect_source_flags
    PICK_IDENTIFIER          = 36,  // [GS] _state.pick_identifier
    LINE_CAP                 = 37,  // [GS+SNAP] _state.line_cap
    LINE_JOIN                = 38,  // [GS+SNAP] _state.line_join
    LINE_TYPE_CONTINUATION   = 39,  // [GS+SNAP] _state.line_type_cont
    LINE_TYPE_INITIAL_OFFSET = 40,  // [GS+SNAP] _state.line_type_offset
    TEXT_SCORE_TYPE          = 41,  // [GS+SNAP] _state.text_score_type
    RESTRICTED_TEXT_TYPE     = 42,  // [GS+SNAP] _state.restricted_text_type
    INTERPOLATED_INTERIOR    = 43,  // [GS+SNAP] _state.interpolated_interior
    EDGE_CAP                 = 44,  // [GS+SNAP] _state.edge_cap
    EDGE_JOIN                = 45,  // [GS+SNAP] _state.edge_join
    EDGE_TYPE_CONTINUATION   = 46,  // [GS+SNAP] _state.edge_type_cont
    EDGE_TYPE_INITIAL_OFFSET = 47,  // [GS+SNAP] _state.edge_type_offset
    SYMBOL_LIB_INDEX         = 48,  // [GS+SNAP] _state.symbol_lib_index
    SYMBOL_COLOUR            = 49,  // [GS+SNAP] _state.symbol_colour
    SYMBOL_SIZE              = 50,  // [GS+SNAP] _state.symbol_size
    SYMBOL_ORIENT            = 51   // [GS+SNAP] _state.symbol_orient

};



enum class Escape : std::uint8_t {

    ESCAPE = 1   // [GS] read identifier + data record, store for inspection

};



enum class External : std::uint8_t {

    MESSAGE  = 1,   // [GS] read action flag + message string, log it
    APP_DATA = 2    // [GS] read identifier + data record, store it

};



enum class Segment : std::uint8_t {

    COPY_SEGMENT             = 1,   // [DOM] render segment at transform → SegmentCopyNode
    INHERITANCE_FILTER       = 2,   // [GS] _state.inheritance_filter
    CLIP_INHERITANCE         = 3,   // [GS] _state.clip_inheritance
    SEGMENT_TRANSFORMATION   = 4,   // [GS] _state.segment_transforms map
    SEGMENT_HIGHLIGHTING     = 5,   // [GS] _state.segment_highlighting map
    SEGMENT_DISPLAY_PRIORITY = 6,   // [GS] _state.segment_display_priority map
    SEGMENT_PICK_PRIORITY    = 7    // [GS] _state.segment_pick_priority map

};



enum class APS : std::uint8_t {

    APS_ATTRIB = 1   // [GS] attach named attribute to current APS node on stack

};


