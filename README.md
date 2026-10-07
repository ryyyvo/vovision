# VoVision

VoVision is a long-term real-time computer vision project built primarily in modern C++ on Linux. It starts small—capturing and displaying webcam frames with OpenCV—and will gradually evolve into a multithreaded, GPU-accelerated vision and tracking system with telemetry, networking, and optional embedded hardware integration.

## Current Status

Milestone 1 is in progress.

Implemented so far:

- C++20 build with CMake + Ninja
- OpenCV integration
- Logitech USB webcam access from WSL2 through V4L2
- Live frame capture and display
- Graceful quit with `q`
- Frame validation and camera error handling
- FPS measurement using `std::chrono`
- On-screen FPS text using `cv::putText`
- Explicit MJPEG / resolution / FPS camera configuration

Current development camera mode:

- Resolution: `1280x720`
- Requested frame rate: `60 FPS`
- Pixel format: MJPEG
- Capture backend: V4L2

> **Environment note:** webcam throughput inside WSL2 is currently limited by the Windows → usbipd → WSL2 USB path. Direct V4L2 testing delivers roughly 15 FPS when requesting 720p60. See [Troubleshooting](docs/TROUBLESHOOTING.md).

## Long-Term Goal

```text
USB Camera
    |
    v
Capture Thread
    |
    v
Bounded Frame Queue
    |
    v
Preprocessing
    |
    v
GPU Inference
    |
    v
Object Tracking
    |
    +------> Renderer
    |
    +------> Telemetry / Networking
```

Later phases may add PyTorch experimentation, CUDA / GPU inference, C++ model deployment, object detection and tracking, multithreaded producer-consumer pipelines, backpressure, telemetry, networking, multiple processes, and optional ESP32 / STM32 control.

## Development Environment

- Windows host
- WSL2
- Ubuntu Linux
- VS Code connected to WSL
- GCC / G++
- CMake
- Ninja
- OpenCV
- V4L2
- Git
- C++20

See [Setup](docs/SETUP.md) for installation and configuration.

## Repository Layout

```text
vovision/
├── CMakeLists.txt
├── README.md
├── docs/
│   ├── ARCHITECTURE.md
│   ├── ROADMAP.md
│   ├── SETUP.md
│   └── TROUBLESHOOTING.md
├── src/
│   └── main.cpp
└── build/                 # generated locally; do not commit
```

More directories such as `include/`, `tests/`, `inference/`, `tracking/`, or `networking/` should only be added when the project actually needs them.

## Build

Configure once:

```bash
cmake -S . -B build -G Ninja
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/vovision
```

During normal development you usually only need:

```bash
cmake --build build
./build/vovision
```

## Current CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.20)

project(VoVision LANGUAGES CXX)

find_package(OpenCV REQUIRED)

add_executable(vovision
    src/main.cpp
)

target_compile_features(vovision PRIVATE cxx_std_20)

target_include_directories(vovision PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(vovision PRIVATE
    ${OpenCV_LIBS}
)
```

## Current Runtime Flow

```text
Open camera
    |
    v
Configure V4L2 / MJPEG
    |
    v
Capture frame
    |
    v
Validate frame
    |
    v
Update FPS measurement
    |
    v
Draw FPS overlay
    |
    v
Display frame
    |
    v
Check keyboard input
    |
    +---- q ----> clean shutdown
    |
    +----------- repeat
```

## Learning Goals

This project is also being used to learn modern C++ and systems programming, including object lifetime, scope, constructors/destructors, RAII, ownership, references, pointers, `const`, STL containers, smart pointers, move semantics, threading, synchronization, producer-consumer design, error handling, and performance profiling.

Where useful, C++ behavior should be compared with equivalent or contrasting behavior in C and Python.

## Next Milestones

1. Grayscale conversion
2. Resizing
3. Cropping / regions of interest
4. Gaussian blur
5. Edge detection
6. Drawing boxes and shapes
7. Inspecting `cv::Mat` dimensions, channels, pixel types, and memory behavior

After that, the project moves into a concurrent video pipeline with separate capture and processing stages.

See [Roadmap](docs/ROADMAP.md) for the larger plan.

## Project Philosophy

VoVision should remain understandable at every phase. Avoid introducing technology simply because it is impressive or résumé-friendly.

```text
understand the concept
        |
        v
build the smallest useful version
        |
        v
test it
        |
        v
break it deliberately
        |
        v
debug it
        |
        v
improve the architecture
```
