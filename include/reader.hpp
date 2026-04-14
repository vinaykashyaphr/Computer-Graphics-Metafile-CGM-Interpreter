# pragma once

# include <string_view>
# include <cstdint>
# include <vector>
# include "schemes/element.hpp"




class Reader {

    const std::string_view _logger;

    public:
        explicit Reader(const std::vector<std::uint8_t>& data);
        ~Reader();

        std::vector<CgmElement> take_elements();

    private:
        struct ElementProps;
        const std::vector<std::uint8_t>& _data;
        std::vector<CgmElement> _elements;

        void read();
        ElementProps get_props(std::size_t& elem_index);
        bool is_partitioned(std::uint16_t& param_len, std::size_t& elem_index);
        void interpret_elems(bool is_continuation, const auto& elem_props, std::size_t& elem_index);

        void log_elements() const;

};


