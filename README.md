# Neville 2.0 framework

### Prerequisites

#### General
- `git`
- `cmake`
- at least 2GB of disk space

#### Windows
- WSL2

#### Linux camera/GUI toolchain
OpenCV's `cv::imshow` and `cv::waitKey` need a GUI backend (GTK) and Video4Linux. Install these once before running `scripts/build-opencv.sh`:

```bash
sudo apt update
sudo apt install -y libgtk-3-dev libv4l-dev pkg-config v4l-utils
```


### How to run your code

First you will need to build the opencv library. To do that, run the following commands after ensuring that you are in the **root** directory of the project.

```
chmod +x scripts/build-opencv.sh
./scripts/build-opencv.sh
```

>[!Note]
>Please note that we are building opencv from scratch. It is a big library, so it is perfectly normal if your computer takes a few minutes to build it. Run the commands above right away and finish reading this while its doing its thing.

After opencv compiles, you will be able to link it to your program. Write your program in `src/main.cpp` and compile the project with

```
mkdir build && cd build
cmake ..
cmake --build .
```

You will only need to do this once to create a place where your compiled executable will live in the future. After it is created, you will have to run `make` from **within** the `build/` directory to **recompile** your code:

```
cd build
make
```

After your code is compiled, you will be able to run the executable with `./build/main` from the root of the project (or just `./main` if you are already in the `build/` directory)

#### Forward USB ports to WSL:
> [!WARNING]  
> You need to share the USB devices everytime you reboot.

> [!IMPORTANT]  
> You need to run Powershell as administrator

```powershell
usbipd list
usbipd bind --busid <x-y>
usbipd attach --wsl --busid <x-y>
```
Replace <x-y> with the busid from usbipd list, without the <>. Repeat this command twice, once for the reciever and once for the transmitter. 

You can verify that this worked by running `usbipd list` again, which should now show `shared` under `STATE`. You should also be able to see it under WSL using `lsusb`.

The reciever device should have a name like `USB Video, USB Digital Audio, USB Input Device`

#### Quick command recap

```bash
# Build OpenCV with GTK/V4L support
chmod +x scripts/build-opencv.sh
./scripts/build-opencv.sh

# Configure & build Neville once
cmake -S . -B build
cmake --build build

# (Optional) recompile after code changes
cmake --build build

# Check serial permissions and groups
./scripts/check-serial-perms.sh

# Run the robot UI (requires GUI/camera access)
# On WSL, you need to run this with sudo, otherwise it will fail to access the reciever
# Ensure that you have BOTH the transmitter and reciever plugged in.
sudo ./build/main
```

### Keyboard controls

Once the OpenCV window is focused, use the following keys to drive Neville:

| Key | Action |
| --- | ------ |
| `w` | Drive forward |
| `s` | Reverse |
| `a` | Turn left (right wheel forward) |
| `d` | Turn right (left wheel forward) |
| `c` | Coast/stop both motors |
| `Esc` or `q` | Stop and exit |

### Finding the USB port

In order to work with Neville, you will need to know which serial device maps to your USB cable:

1. Plug Neville in and watch the kernel logs for new devices:
	```bash
	dmesg | grep -i tty
	```
2. List detected devices and their permissions:
	```bash
	ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
	```
3. Run the helper script (defaults to `/dev/ttyUSB0`, override with an argument or `NEVILLE_SERIAL_PORT`):
	```bash
	./scripts/check-serial-perms.sh
	./scripts/check-serial-perms.sh /dev/ttyUSB0
	```

Set the `NEVILLE_SERIAL_PORT` environment variable if you need the application to talk to a non-default device:

```bash
export NEVILLE_SERIAL_PORT=/dev/ttyUSB0
```

### Fixing `serial::IOException (13) Permission denied`

Linux protects serial devices such as `/dev/ttyUSB0` by restricting access to the `dialout` group (or the group listed by `ls -l /dev/ttyUSB0`). If you see `IO Exception (13): Permission denied` when running `./main`, grant your user access:

```bash
sudo usermod -aG dialout $USER
newgrp dialout  # or log out and back in / reboot once
```

You can temporarily grant access until the next reboot/unplug by running `sudo chmod a+rw /dev/ttyUSB0`, but adding your user to `dialout` is the recommended long-term fix.

### Camera configuration & GUI troubleshooting

- The application uses camera index `0` by default. Override it with:
	```bash
	export NEVILLE_CAMERA_INDEX=1
	```
- If you see `OpenCV(...): The function is not implemented. Rebuild the library with ... GTK+ ...`, install the packages listed above and re-run `./scripts/build-opencv.sh` so that OpenCV is compiled with GTK/V4L support (`-DWITH_GTK=ON -DWITH_V4L=ON`).
- When a camera cannot be opened, double-check which `/dev/video*` node is created (e.g., with `v4l2-ctl --list-devices`) and ensure no other process is using it.
- Running inside WSL requires either WSLg (Windows 11) or an X server on Windows. Make sure GUI forwarding works before launching `./build/main`.