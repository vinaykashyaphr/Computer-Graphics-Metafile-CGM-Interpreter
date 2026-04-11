# include <cstdint>
# include <fstream>
# include <vector>
# include <string_view>
# include <filesystem>


namespace fs = std::filesystem;




class CgmLoader {

    std::ifstream _stream;
    const std::string_view _logger;

    void log_stream(std::vector<std::uint8_t>& buffer);

    public:
        explicit CgmLoader(const fs::path& file_path, const std::string_view logger);
        std::unique_ptr<std::vector<std::uint8_t>> load();

};


