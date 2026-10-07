# Troubleshooting

## Webcam Disappears After Restarting WSL

If:

```bash
ls -l /dev/video*
```

returns no video devices, reattach the webcam to the running WSL2 VM.

From Windows PowerShell:

```powershell
usbipd list
usbipd attach --wsl --busid <BUSID>
```

Then verify in WSL:

```bash
lsusb
ls -l /dev/video*
```

## OpenCV Cannot Be Found

If:

```bash
pkg-config --modversion opencv4
```

cannot find OpenCV:

```bash
sudo apt update
sudo apt install -y libopencv-dev pkg-config
```

## Camera Opens But Frame Capture Fails

Check the device directly:

```bash
v4l2-ctl --list-devices
v4l2-ctl --device=/dev/video0 --all
v4l2-ctl --device=/dev/video0 --list-formats-ext
```

Application-side validation should distinguish:

```text
camera initialization failed
frame capture failed
frame data is empty
```

## Low Webcam FPS Under WSL2

Observed direct V4L2 throughput in the current environment:

```text
1280x720 MJPEG requested @ 30 FPS -> ~7.5 FPS delivered
1280x720 MJPEG requested @ 60 FPS -> ~15 FPS delivered
640x480  MJPEG requested @ 30 FPS -> ~7.5 FPS delivered
```

The limitation was isolated below OpenCV using:

```bash
v4l2-ctl \
    --device=/dev/video0 \
    --set-fmt-video=width=1280,height=720,pixelformat=MJPG \
    --set-parm=60 \
    --stream-mmap \
    --stream-count=300 \
    --stream-to=/dev/null
```

This still delivered only about 15 FPS, showing that the application and OpenCV display loop are not the primary bottleneck.

For now, use the stable 720p60-requested MJPEG mode and treat the delivered ~15 FPS as an environment constraint.

Possible future alternatives:

- native Linux capture
- native Windows capture with frames transported to the WSL process
- revisiting USB forwarding if the WSL/usbipd environment changes

## Corrupt JPEG Data

Observed messages include:

```text
Corrupt JPEG data: premature end of data segment
Corrupt JPEG data: extraneous bytes before marker ...
```

These indicate damaged or incomplete MJPEG data reaching the decoder.

In the current environment, this was more common with the 720p30 configuration. Requesting 720p60 produced a cleaner stream even though actual delivered FPS remains below the requested rate.

## `cv::waitKey(0)` Does Not React

The OpenCV display window must have keyboard focus.

For live video, use a small positive delay:

```cpp
int key = cv::waitKey(1);
```

Then:

```cpp
if (key == 'q') {
    break;
}
```

## `fps_text` Is Out of Scope

This does not work:

```cpp
if (condition) {
    std::string fps_text = "...";
}

cv::putText(frame, fps_text, ...);
```

`fps_text` only exists inside the `if` block.

Instead:

```cpp
double fps{0.0};
std::string fps_text{"FPS: 0.0"};
```

Then update the existing variable:

```cpp
fps_text = std::format("FPS: {:.1f}", fps);
```

The difference is:

```cpp
std::string fps_text = ...;  // declaration
fps_text = ...;              // assignment
```
