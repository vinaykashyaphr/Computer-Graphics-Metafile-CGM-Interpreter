# pragma once



struct Flags {

    // Major states: reference

    bool in_metafile = false;
    bool in_picture = false;
    bool in_picture_body = false;
    bool in_segment = false;
    int aps_depth = 0;       // incremented per BEGIN_APP_STRUCTURE, decremented per END
    int aps_body_depth = 0;  // incremented per BEGIN_APP_STRUCTURE_BODY, decremented per END
    bool in_defaults_replacement = false;


    // Minor states: reference

    bool in_figure = false;
    bool in_text = false;
    bool in_compound_path = false;
    bool in_protection_region = false;
    bool in_tile_array = false;

    // major states — computed on demand

    // Metafile Closed State
    bool MCS() const {
        return !in_metafile;
    }

    // Picture Closed State
    bool PCS() const {
        return in_metafile && !in_picture && !in_picture_body;
    }

    // Metafile Descriptor State
    bool MDS() const {
        return in_metafile && !in_picture && !in_picture_body && !in_segment;
    }

    // Picture Description State
    bool PDS() const {
        return in_metafile && in_picture && !in_picture_body && !in_segment;
    }

    // Picture Open State
    bool POS() const {
        return in_metafile && in_picture && in_picture_body && !in_segment;
    }

    // Structure Descriptor State
    bool SDS() const {
        return in_metafile && in_picture && in_picture_body && (aps_depth > aps_body_depth);
    }

    // Structure Open State
    bool SOS() const {
        return in_metafile && in_picture && in_picture_body && (aps_body_depth > 0);
    }

    // Global Segment State
    bool GSS() const {
        return in_metafile && in_segment && !in_picture;
    }

    // Picture Discriptor Segment State
    bool DSS() const {
        return in_metafile && in_picture && in_segment && !in_picture_body;
    }

    // Local Segment State
    bool LSS() const {
        return in_metafile && in_picture && in_picture_body && in_segment;
    }


    bool DR()  const { return in_metafile && in_defaults_replacement; }

    
    // minor states — computed on demand
    bool FOS() const {
        return in_figure;
    }

    bool PRS() const {
        return in_protection_region;
    }

    bool CPS() const {
        return in_compound_path;
    }

    bool TOS() const { return in_text; }
    bool TAS() const { return in_tile_array; }

};