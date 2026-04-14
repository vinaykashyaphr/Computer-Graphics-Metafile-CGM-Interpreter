# pragma once

# include <cstdint>
# include <fstream>
# include <vector>
# include <string_view>
# include <filesystem>


namespace fs = std::filesystem;




class CGM {

    std::ifstream _stream;
    const std::string_view _logger;

    void log_stream(std::vector<std::uint8_t>& buffer);

    public:
        explicit CGM(const fs::path& file_path);
        std::unique_ptr<std::vector<std::uint8_t>> load();

};


