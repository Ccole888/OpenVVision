#include "Run.h"
using PatternKey = uint16_t;              
using PatternMap = std::unordered_map<PatternKey, size_t>;

int main (int argc, char* argv[]) {
    Args args  = parseArgs( argc, argv);
    std::vector<uint8_t> frameBuffer ( FRAME_SIZE );

    PatternMap patternTable;
    patternTable.reserve ( PATTERN_ENTRIES );

    
    while(true) {
        // Fill with white
        std::fill ( frameBuffer.begin (), frameBuffer.end (), 0xFF );

        // Dummy pattern extraction
        for(size_t i = 0; i < PATTERN_ENTRIES; ++i) {
            PatternKey key = static_cast<PatternKey> ( i & 0x7FFF );
            patternTable[key] = i;
        }

        // Wrap raw bytes into an OpenCV Mat
        cv::Mat frame ( FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, frameBuffer.data () );

        // Display
        cv::imshow ( "Frame", frame );

        // Wait for keypress (break after one frame)
        cv::waitKey ( 0 );
        break;
    }
}
















Args parseArgs ( int argc, char *argv[] ) {
    Args args;

    for(int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if(arg == "-v") {
            args.mode = VisualMode::Video;
        }
        else if(arg == "-g") {
            args.mode = VisualMode::GPS;
        }
        else if(arg == "-vg" || arg == "-gv") {
            args.mode = VisualMode::VideoGPS;
        }
        else if(arg == "-s") {
            if(i + 1 >= argc)
                throw std::runtime_error ( "Error: -s requires a value" );
            args.serial = argv[++i];
        }
        else if(arg == "-h") {
            printHelp ( argv[0] );
            exit ( 0 );
        }
        else if(arg == "-gui") {
            args.GUI = true;
        }
        else {
            throw std::runtime_error ( "Error: unknown argument: " + arg );
        }
    }

    return args;
}