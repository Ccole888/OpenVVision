#include <vector>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

constexpr int FRAME_WIDTH = 1280;
constexpr int FRAME_HEIGHT = 720;
constexpr int CHANNELS = 3;
constexpr size_t FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT * CHANNELS;  // 2,764,800 bytes

// For the pattern lookup table (assuming 510-bit sequence, 15-bit window map)
constexpr size_t PATTERN_ENTRIES = 500;  // ~500 positions
enum class Mode : uint8_t {
	VISION,
	GPS,
	VISIONGPS,
	NONE
};
Mode CurrentMode = Mode::NONE;

enum class VisualMode { None, Video, GPS, VideoGPS };

struct Args {
    VisualMode mode = VisualMode::None;
    std::string serial;         // optional serial port
    bool GUI;
};

void printHelp ( const char *programName ) {
    std::cout << "Usage: " << programName << " [options]\n\n"
        << "Options:\n"
        << "  -v        Video mode\n"
        << "  -g        GPS mode\n"
        << "  -gui      GUI mode\n"
        << "  -vg/-gv   Video + GPS mode\n"
        << "  -s <port> Serial port (optional, e.g. COM3)\n"
        << "  -h        Show this help message\n";
}

Args parseArgs ( int argc, char *argv[] );