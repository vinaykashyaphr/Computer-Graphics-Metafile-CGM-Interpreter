# pragma once

# include <fstream>

# include "schemes/element.hpp"
# include "schemes/graphics_state.hpp"




class _Delimiter {

    const CgmElement& _elem;
    GraphicsState& _state;
    std::ofstream& _log;

    public:
        _Delimiter(GraphicsState& state, const CgmElement& elem, std::ofstream& log);
        void dispatch();

    private:
        void _begin_metafile();


};
