# pragma once

# include <string_view>
# include <vector>
# include <unordered_map>
# include <memory>

# include "schemes/element.hpp"
# include "schemes/graphics_state.hpp"
# include "dom/node.hpp"




class Dispatcher {

    const std::vector<CgmElement>& _elements;
    GraphicsState _state;
    const std::string_view _logger;
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>> _segment_store;
    std::unordered_map<std::uint32_t, std::vector<std::unique_ptr<Node>>> _protection_regions;

    public:
        Dispatcher(const std::vector<CgmElement>& elements);
        void dispatch();

    private:
        void dispatch_delimiter(const CgmElement& el);
        void dispatch_metafile_desc(const CgmElement& el);
        void dispatch_picture_desc(const CgmElement& el);
        void dispatch_control(const CgmElement& el);
        void dispatch_geometry(const CgmElement& el);
        void dispatch_attribute(const CgmElement& el);
        void dispatch_segment(const CgmElement& el);
        void dispatch_aps_desc(const CgmElement& el);
        void log_unknown(const CgmElement& el);


};


