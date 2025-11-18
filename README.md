# Neville 2.0 framework

### Prerequisites

#### General
- `git`
- `cmake`
- at least 2GB of disk space

#### Windows
- WSL2 [See warion-dev branch for latest patches]


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

### Finding the USB port

In order to work with the Neville, you will need to 
