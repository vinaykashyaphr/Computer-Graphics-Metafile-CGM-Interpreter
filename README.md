# cgm_interpreter

A C++23 interpreter for binary CGM (Computer Graphics Metafile) files, targeting conversion to SVG. Implements the ISO/IEC 8632 standard (parts 1, 3, and 4).

## What is CGM?

CGM (Computer Graphics Metafile) is an ISO/IEC 8632 standard for storing and exchanging 2D vector graphics. It is widely used in technical documentation (aviation, defense, engineering). A CGM file encodes graphics as a stream of typed elements organized into classes — delimiters, descriptors, geometry primitives, attributes, and more.

This interpreter targets the **binary encoding** defined in ISO/IEC 8632-3.

## Architecture

The pipeline has three stages:

```
<input.cgm>
    │
    ▼
 CGM (Loader)          — reads the raw binary file into a byte buffer
    │
    ▼
 Reader                — parses the buffer into a flat list of CgmElements
    │                    (handles long-form partitioning and even-byte padding)
    ▼
 Dispatcher            — routes each element by class to a specialized handler
    │
    ├── _Delimiter          (class 0 — structural: begin/end metafile, picture, segment…)
    ├── _MetafileDescriptor (class 1 — precision, fonts, colour model, defaults…)
    ├── [Picture Descriptor] (class 2 — viewport, VDC extent, background colour…)
    ├── [Control]            (class 3 — clipping, transparency, save/restore context…)
    ├── [Geometry]           (class 4 — polyline, polygon, text, arcs, splines, NURBS…)
    ├── [Attribute]          (class 5 — line/fill/text/edge/marker style…)
    └── …
         │
         ▼
      DOM nodes  →  SVG output
```

### Key components

| Component | File(s) | Role |
|---|---|---|
| `CGM` | [include/loader.hpp](include/loader.hpp), [src/loader.cpp](src/loader.cpp) | Reads `.cgm` file as raw bytes; validates extension |
| `Reader` | [include/reader.hpp](include/reader.hpp), [src/reader.cpp](src/reader.cpp) | Decodes binary element stream into `CgmElement` structs; handles long-form and partitioned elements |
| `CgmElement` | [include/schemes/element.hpp](include/schemes/element.hpp) | Plain struct: `elem_class`, `elem_id`, raw `params` bytes |
| `Dispatcher` | [include/dispatchers/dispatcher.hpp](include/dispatchers/dispatcher.hpp), [src/dispatchers/dispatcher.cpp](src/dispatchers/dispatcher.cpp) | Routes elements to class handlers; replays metafile defaults on `BEGIN_PICTURE` |
| `_Delimiter` | [include/dispatchers/delimiter.hpp](include/dispatchers/delimiter.hpp), [src/dispatchers/delimiter.cpp](src/dispatchers/delimiter.cpp) | Handles class 0 structural elements |
| `_MetafileDescriptor` | [include/dispatchers/metafile_desc.hpp](include/dispatchers/metafile_desc.hpp), [src/dispatchers/metafile_desc.cpp](src/dispatchers/metafile_desc.cpp) | Handles class 1 — precision fields, font list, colour model |
| `State` | [include/schemes/state.hpp](include/schemes/state.hpp), [src/schemes/state.cpp](src/schemes/state.cpp) | Graphics state machine; holds all current rendering attributes with ISO 8632-1 defaults |
| `param_reader` | [include/param_reader.hpp](include/param_reader.hpp), [src/param_readers.cpp](src/param_readers.cpp) | Type-aware binary decoder (integer, real, enum, colour, point, string, VDC…) |
| `Node` | [include/dom/node.hpp](include/dom/node.hpp) | Abstract base for DOM nodes; each geometry element implements `to_svg()` |

## Binary Encoding

CGM binary elements are encoded as big-endian 16-bit words:

```
Bits 15–12  element class  (4 bits)
Bits 11–5   element id     (7 bits)
Bits 4–0    param length   (5 bits, or 31 = "long form")
```

When the 5-bit length field is `31`, the next word carries the real length:
- Bit 15 = "not last partition" flag (element spans multiple fragments)
- Bits 14–0 = actual byte count for this fragment

Parameter bytes are always padded to an even boundary.

## CGM Element Classes

| Class | Name | Handler status |
|---|---|---|
| 0 | Delimiter | implemented |
| 1 | Metafile Descriptor | implemented |
| 2 | Picture Descriptor | in progress |
| 3 | Control | planned |
| 4 | Geometry | planned |
| 5 | Attribute | planned |
| 6 | Escape | planned |
| 7 | External | planned |
| 8 | Segment | planned |
| 9 | APS (Application Structure) | planned |

### Geometry primitives (class 4)

Polyline, disjoint polyline, polymarker, text, restricted text, append text, polygon, polygon set, cell array, rectangle, circle, circular arcs (3-point and centre forms), ellipse, elliptical arcs, conic arcs, non-uniform B-spline, NURBS, polybezier, polysymbol, bitonal tile, tile.

### Attributes (class 5)

Line (type, width, colour, cap, join, continuation), marker (type, size, colour), fill (interior style, colour, hatch, pattern), edge (type, width, colour, visibility, cap, join), text (font, precision, colour, height, orientation, path, alignment, spacing), symbol, colour table, aspect source flags.

## Graphics State

`State` tracks the full graphics state machine as defined in ISO 8632-1 clause 8, including:

- Precision fields (`int_bytes`, `real_bytes`, `index_bytes`, `colour_bytes`, etc.)
- VDC type (integer or real) and extents
- Colour model (RGB, CMYK, CIELAB, CIELUV, RGB-related) via `std::variant`
- Colour table, font list, character set list
- All attribute defaults (line, fill, edge, text, marker, symbol)
- Clip region and clip indicator
- Bundle tables (line, marker, text, fill, edge)
- Custom dash patterns, hatch styles, geometric patterns
- Segment transforms, highlighting, and display priority
- APS (Application Structure) stack
- Save/restore primitive context stack (`Snapshot`)

`State::Snapshot` captures the drawable-style subset of state for attaching to DOM nodes at the moment a geometry element is encountered.

## Logging

Three log files are written to `_logs/` on every run:

| File | Content |
|---|---|
| `_logs/loader.log` | Hex dump of the raw binary file |
| `_logs/reader.log` | Decoded element list: class, id, param_len |
| `_logs/dispatcher.log` | Dispatch trace and handler output |

## Build

Requires CMake 3.20+ and a C++23-capable compiler.

```sh
cmake -B build
cmake --build build
```

## Usage

```sh
./build/cgm_interpreter <input.cgm>
```

## References

- ISO/IEC 8632-1:1999 — CGM Functional Specification
- ISO/IEC 8632-3:1999 — CGM Binary Encoding
- ISO/IEC 8632-4:1999 — CGM Clear Text Encoding
- ISO/IEC 8632-1:1999/Cor 1:2006, Cor 2:2007 — Technical Corrigenda
