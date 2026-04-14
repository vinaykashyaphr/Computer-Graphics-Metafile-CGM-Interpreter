# include <iostream>
# include <filesystem>

# include "dispatchers/dispatcher.hpp"
# include "loader.hpp"
# include "reader.hpp"
# include "schemes/element.hpp"


namespace fs = std::filesystem;



int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.cgm>\n";
        return 1;
    }

    fs::path input  = argv[1];

    CGM loader(input);
    auto buffer = loader.load();

    std::vector<CgmElement> elements;

    {
        Reader reader(*buffer);
        elements = reader.take_elements();
    }

    Dispatcher dispatcher(elements);
    dispatcher.dispatch();

    // for (auto& el : elements) {
    //     printf("class=%d id=%d params=%zu bytes\n",
    //            el.elem_class, el.elem_id, el.params.size());
    // }

}


