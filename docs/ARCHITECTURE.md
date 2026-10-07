# Architecture Notes

This document describes the architecture VoVision has today and how it is expected to evolve.

The project intentionally avoids implementing the final architecture up front.

## Current Architecture

```text
Logitech USB Camera
        |
        v
Windows USB Stack
        |
        v
usbipd-win
        |
        v
WSL2 / Linux
        |
        v
V4L2
        |
        v
OpenCV VideoCapture
        |
        v
cv::Mat frame
        |
        +--> FPS measurement
        |
        +--> text overlay
        |
        v
OpenCV HighGUI display
```

Inside the application:

```text
Initialization
    |
    +--> open camera
    +--> validate camera
    +--> configure MJPEG / resolution / FPS
    +--> initialize timing state
    |
    v
Main Loop
    |
    +--> capture frame
    +--> validate frame
    +--> update FPS
    +--> modify frame / overlay text
    +--> display frame
    +--> process keyboard input
    |
    v
Cleanup
```

## Current Resource-Lifetime Model

Important C++ objects currently include:

- `cv::VideoCapture camera`
- `cv::Mat frame`
- `std::string fps_text`
- `std::chrono` time points and durations

`cv::VideoCapture` and `cv::Mat` are C++ objects whose destructors participate in resource cleanup when they leave scope.

This is an early example of RAII:

```text
object lifetime = resource lifetime
```

This becomes increasingly important when the project adds files, sockets, mutexes, threads, queues, CUDA memory, and inference runtimes.

## Current Frame Processing Model

One `cv::Mat` variable is reused by the capture loop:

```text
iteration 1 -> frame contains camera frame #1
iteration 2 -> frame updated with camera frame #2
iteration 3 -> frame updated with camera frame #3
```

OpenCV operations such as `cv::putText` mutate the image passed to them, so ordering matters:

```text
capture -> process / annotate -> display
```

Later work should examine `cv::Mat` reference counting, shallow copies, deep copies, ownership, and safe transfer between threads.

## Next Architectural Step

After the basic OpenCV phase:

```text
Capture Thread
      |
      v
Bounded Frame Queue
      |
      v
Processing Thread
      |
      v
Result / Display
```

The purpose is not merely to use threads. It is to learn what happens when stages run at different rates.

Example:

```text
camera capture: 60 FPS
processing:     30 FPS
```

Future backpressure strategies include bounded queues, dropping old/new frames, latest-frame-only processing, and blocking producers.

## Future Vision Architecture

```text
                      +----------------+
USB Camera ---------->| Capture Thread |
                      +-------+--------+
                              |
                         Bounded Queue
                              |
                      +-------v--------+
                      | Preprocessing  |
                      +-------+--------+
                              |
                         GPU Tensor
                              |
                      +-------v--------+
                      | GPU Inference  |
                      +-------+--------+
                              |
                         Detections
                              |
                      +-------v--------+
                      | Object Tracker |
                      +-------+--------+
                              |
                    +---------+---------+
                    |                   |
                    v                   v
                Renderer            Event Queue
                                        |
                                        v
                                Network / API Layer
```

Do not implement this architecture until the simpler pipeline creates a real need for it.

## Design Principles

1. Prefer measurable behavior over assumptions.
2. Add concurrency only when there is a concrete pipeline reason.
3. Keep ownership and lifetime explicit.
4. Fail near the source of invalid data.
5. Centralize shutdown and cleanup paths where practical.
6. Measure latency and throughput independently.
7. Avoid framework complexity until the current design becomes limiting.
