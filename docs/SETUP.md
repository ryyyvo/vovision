# Development Setup

This guide documents the development environment currently used for VoVision.

## Host Environment

VoVision currently runs inside Ubuntu on WSL2 using a Windows desktop as the host machine.

```text
Windows
  |
  v
WSL2
  |
  v
Ubuntu Linux
  |
  +--> GCC / G++
  +--> CMake
  +--> Ninja
  +--> OpenCV
  +--> V4L2
```

VS Code can be connected directly to WSL for editing and debugging.

## Verify WSL

From Windows PowerShell:

```powershell
wsl --status
wsl --list --verbose
```

The Ubuntu distribution should be running under WSL version 2.

## Install Build Tools

Inside Ubuntu / WSL:

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    pkg-config
```

Optional but useful tools:

```bash
sudo apt install -y \
    clang \
    gdb \
    valgrind \
    strace \
    ltrace
```

Verify:

```bash
g++ --version
cmake --version
ninja --version
git --version
```

## Install OpenCV

```bash
sudo apt install -y libopencv-dev
```

Verify:

```bash
pkg-config --modversion opencv4
```

The current environment reports OpenCV `4.10.0`.

## Install V4L2 Utilities

```bash
sudo apt install -y v4l-utils
```

Useful commands:

```bash
v4l2-ctl --list-devices
v4l2-ctl --device=/dev/video0 --all
v4l2-ctl --device=/dev/video0 --list-formats-ext
```

## Attach the Webcam to WSL2

The webcam is currently passed into WSL through `usbipd-win`.

From Windows PowerShell:

```powershell
usbipd list
```

If the camera is already shared:

```powershell
usbipd attach --wsl --busid <BUSID>
```

If the device is not yet shared, use Administrator PowerShell first:

```powershell
usbipd bind --busid <BUSID>
```

Then attach it:

```powershell
usbipd attach --wsl --busid <BUSID>
```

Back inside WSL:

```bash
lsusb
ls -l /dev/video*
```

A successfully attached camera should expose devices such as `/dev/video0` and `/dev/video1`.

### Important

The USB attachment may need to be repeated after:

- `wsl --shutdown`
- Windows restart
- USB disconnect/reconnect
- some device resets

If `/dev/video*` disappears, re-run the `usbipd attach` command.

## Camera Permissions

Check:

```bash
ls -l /dev/video0
id
```

If necessary:

```bash
sudo usermod -aG video $USER
```

Then restart the WSL session and reattach the webcam.

## Project Layout

```text
vovision/
├── CMakeLists.txt
├── README.md
├── docs/
└── src/
    └── main.cpp
```

## Configure and Build

Configure:

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

## Typical Development Loop

```text
edit source
    |
    v
cmake --build build
    |
    v
./build/vovision
    |
    v
observe / debug
    |
    +------ repeat
```
