#pragma once
// mata_SDK 7

//////////////////////////////////// Window options
// window name
constexpr const char* WINDOW_NAME = "mata_SDK 7";

// window size
constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 500;

// fullscreen option
constexpr bool FULL_SCREEN_OPTION = false;

// When enabled, the window do not react to ALT key press event.
constexpr bool DISABLE_ALT_EVENT = true;



//////////////////////////////////// SDK options
// intro screen option
constexpr bool ENABLE_INTRO_SCREEN = false;

// frame limits option
// If this option is set to 0, the system will run at maximum framerate.
constexpr int FRAME_LIMITS = 0;

// When enabled, the GPU computes matrices. When disabled, the CPU computes matrices.
constexpr bool USE_COMPUTE_SHADER = false;



//////////////////////////////////// Development options
// console window option
constexpr bool SHOW_CONSOLE = true;

// FPS indicator option
constexpr bool SHOW_FPS = false;

// show boundbox option
constexpr bool SHOW_BOUND_BOX = false;

// show frustum bound option
constexpr bool SHOW_FRUSTUM_BOUND = false;

// bound box thickness option
constexpr float BOUND_BOX_THICKNESS = 0.01;

// Program termination options when developing a program
// When this option is activated, pressing ESC will immediately terminate the program.
constexpr bool ENABLE_DEV_EXIT = false;



//////////////////////////////////// Scene Options
// scene layer option
// 'EOL' should be typed at last
enum SDK_LAYER
{ LAYER_BG, LAYER1, LAYER2, LAYER3, LAYER4, LAYER5, LAYER6, LAYER7, EOL };

// Specifies the size of the deletion location buffer
constexpr int DELETE_LOCATION_BUFFER_SIZE = 500;



//////////////////////////////////// Sound Options
// Max sound channel option
constexpr int MAX_CHANNEL_SIZE = 32;

// FFT Size option
constexpr int FFT_SIZE = 1024;



//////////////////////////////////// SDK_Math Options
//When the relevant threshold is reached in SDK_Math::Lerp(), the calculation is stopped immediately to optimize the amount of calculation.
//When set to 0.0, calculation amount optimization is not performed.
constexpr float LERP_THRESHOLD = 0.001;



//////////////////////////////////// Image render Option
// Set the number of blur filtering times to run when blurring images. The higher the value, the more resources the system consumes.
constexpr int BLUR_EXECUTION = 10;



//////////////////////////////////// File security Options
// Application version
constexpr float APPLICATION_VERSION = 1.0;

// Data file security option
// When this option is activated, the data file is saved as an encrypted file.
// When this option is disabled, the data file is saved as an xml file.
constexpr bool USE_FILE_SECURITY = false;

// AES, IV for Data
// Recommend that you make this part private or 
// modify it with the default password before distributing
constexpr unsigned char AES_KEY[16]
{ "1234567890" };

constexpr unsigned char IV_KEY[16]
{ "1234567890" };