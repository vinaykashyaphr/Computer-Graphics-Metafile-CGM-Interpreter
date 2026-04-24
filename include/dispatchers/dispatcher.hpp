# pragma once

# include <fstream>
# include <string_view>
# include <vector>

# include "schemes/element.hpp"
# include "schemes/state.hpp"





class Dispatcher {

    const std::vector<CgmElement>& _elements;
    State _state;
    const std::string_view _logger;

    public:
        Dispatcher(const std::vector<CgmElement>& elements);
        void dispatch();
        void dispatch_element(const CgmElement& el, std::ofstream& log);


};


