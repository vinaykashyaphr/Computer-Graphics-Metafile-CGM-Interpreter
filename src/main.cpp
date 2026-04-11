# include <iostream>
# include <filesystem>

# include "loader.hpp"
# include "reader.hpp"


namespace fs = std::filesystem;



int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.cgm>\n";
        return 1;
    }

    fs::path input  = argv[1];

    CgmLoader loader(input, "out.log");
    auto buffer = loader.load();

    ElementReader reader(*buffer);
    auto elements = reader.elements();

    for (auto& el : elements) {
        printf("class=%d id=%d params=%zu bytes\n",
               el.elem_class, el.elem_id, el.params.size());
    }

}


