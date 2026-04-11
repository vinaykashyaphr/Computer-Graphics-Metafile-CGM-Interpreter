# include <cstdint>
# include <vector>




class ElementReader {

    public:
        explicit ElementReader(const std::vector<std::uint8_t>& data);
        ~ElementReader();

        struct CgmElement {

            std::uint8_t elem_class;
            std::uint8_t elem_id;
            std::vector<std::uint8_t> params;

        };

        std::vector<CgmElement> elements() const;

    private:

        struct ElementProps;
        const std::vector<std::uint8_t>& _data;
        std::vector<CgmElement> _elements;

        void read();
        ElementProps get_props(std::size_t& elem_index);
        bool is_partitioned(std::uint16_t& param_len, std::size_t& elem_index);
        void interpret_elems(bool is_continuation, const auto& elem_props, std::size_t& elem_index);

};