# include <cstddef>
# include <ios>
# include <iomanip>
# include <stdexcept>
# include <memory>

# include "loader.hpp"




CGM::CGM(const fs::path& file_path): 
    _stream(std::ifstream(file_path.string(), std::ios::binary)),
    _logger("_logs/loader.log")
{

    if (!(file_path.extension() == ".cgm" || file_path.extension() == ".CGM")) {
        throw std::runtime_error("Invalid File: " + file_path.string());
    }

    if (!_stream) throw std::runtime_error("Cannot Open: " + file_path.string());

};



std::unique_ptr<std::vector<std::uint8_t>> CGM::load() {

    _stream.seekg(0, std::ios::end);
    std::size_t size = _stream.tellg();
    _stream.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> buffer(size);
    _stream.read(reinterpret_cast<char*>(buffer.data()), size);

    log_stream(buffer);

    return std::make_unique<std::vector<uint8_t>>(std::move(buffer));

}



void CGM::log_stream(std::vector<std::uint8_t>& buffer) {

    std::ofstream log(_logger.data());

    for (std::size_t i = 0; i < buffer.size(); ++i) {
        log << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
        if ((i + 1) % 16 == 0) log << '\n';
    }

    log << '\n';

}


