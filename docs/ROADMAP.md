# Project Roadmap

VoVision will evolve gradually from a simple C++ webcam program into a real-time vision and tracking system.

## Phase 0 — C++ Foundations

Learn concepts as the project requires them rather than studying them in isolation.

Key topics:

- source and header files
- compilation and linking
- CMake
- namespaces
- references and pointers
- `const`
- stack vs heap
- structs and classes
- constructors / destructors
- RAII
- ownership and lifetime
- `std::string`
- STL containers
- smart pointers
- move semantics
- error handling
- lambdas
- threading primitives

Already encountered:

- namespaces (`std::`, `cv::`)
- constructors
- destructors / automatic cleanup
- RAII
- block scope
- brace initialization
- `auto`
- operator overloading
- `std::string`
- `std::chrono`

## Phase 1 — Webcam + Basic OpenCV

### Completed / In Progress

- [x] Open webcam from C++
- [x] Verify camera initialization
- [x] Capture frames
- [x] Validate frames
- [x] Display live video
- [x] Quit with `q`
- [x] Measure application FPS
- [x] Render FPS text onto frames
- [x] Configure MJPEG / resolution / requested FPS
- [x] Investigate WSL webcam throughput limitation

### Next

- [ ] Grayscale conversion
- [ ] Resize frames
- [ ] Crop / region-of-interest operations
- [ ] Gaussian blur
- [ ] Edge detection
- [ ] Draw boxes and shapes
- [ ] Inspect pixel values and types
- [ ] Understand BGR vs RGB
- [ ] Understand `cv::Mat` ownership / shallow copy / deep copy

## Phase 2 — Concurrent Video Pipeline

```text
Capture Thread
      |
      v
Frame Queue
      |
      v
Processing Thread
      |
      v
Display / Result Queue
```

Learn:

- `std::thread`
- `std::mutex`
- `std::condition_variable`
- producer / consumer
- thread-safe queues
- shutdown coordination
- race conditions
- deadlocks
- ownership between threads

Potential component:

```cpp
template <typename T>
class BlockingQueue {
    // push
    // pop
    // bounded capacity
    // shutdown
};
```

## Phase 3 — Backpressure and Real-Time Behavior

Study what happens when stages run at different rates.

Metrics:

- capture FPS
- processing FPS
- queue depth
- dropped frames
- end-to-end latency

Compare bounded queues, dropping policies, latest-frame-only processing, and producer blocking.

## Phase 4 — PyTorch Fundamentals

Use Python first for ML experimentation.

Learn tensors, CPU vs GPU tensors, CUDA availability, model loading, inference mode, preprocessing, confidence scores, and bounding boxes.

Prefer pretrained models initially.

## Phase 5 — Real-Time Object Detection

Prototype object detection and display bounding boxes, labels, confidence, FPS, and inference latency.

## Phase 6 — Performance Benchmarking

Measure capture latency, preprocessing latency, CPU → GPU transfer, inference latency, postprocessing latency, rendering latency, and end-to-end latency.

Compare CPU/GPU execution, resolutions, model sizes, queue strategies, and synchronous/asynchronous processing.

## Phase 7 — C++ Inference Integration

Move inference into C++ after the ML pipeline is understood.

Evaluate then-current deployment options such as ONNX, ONNX Runtime, TensorRT, and current PyTorch export/deployment tooling.

## Phase 8 — CUDA C++

Use CUDA directly when it solves a real performance problem.

Potential early work:

- normalization
- BGR → RGB
- image transforms
- element-wise operations
- preprocessing kernels

Later:

- streams
- asynchronous copies
- pinned memory
- overlapping transfer and compute
- profiling

## Phase 9 — Object Tracking

Progress from simple to complex:

1. centroid tracking
2. Kalman filter
3. Hungarian assignment
4. SORT
5. more advanced tracking if justified

## Phase 10 — Networking / Telemetry

Potential endpoints:

```text
GET /status
GET /detections
GET /metrics
GET /snapshot
```

Learn TCP, sockets, HTTP, serialization, concurrent clients, timeouts, connection management, and telemetry streaming.

## Phase 11 — Multi-Process / Distributed Evolution

Potential processes:

```text
Vision Node
Telemetry Collector
API Server
Dashboard Backend
```

Topics include heartbeats, retries, reconnects, buffering, timestamps, duplicate handling, health monitoring, service discovery, and failure handling.

## Optional — Microcontroller Integration

Potential hardware:

- ESP32
- STM32
- servos
- sensors

Possible extension:

```text
Camera
  |
  v
VoVision
  |
  | USB serial / UART
  v
ESP32 / STM32
  |
  +--> pan servo
  +--> tilt servo
```

## Immediate Next Task

Continue Phase 1 with grayscale conversion and use it to learn:

- image channels
- `cv::Mat` representation
- BGR versus grayscale
- input/output matrices
- mutation versus separate output images
- image type and memory implications
