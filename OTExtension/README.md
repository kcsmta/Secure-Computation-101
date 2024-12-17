## Follow this [official repo](https://github.com/encryptogroup/OTExtension).


## Installation
It is recommended to download and build the library in a partition with sufficient storage space.

### Install dependencies
The OTExtension library relies on several dependencies for arithmetic computation, cryptographic functions, and other utilities. Ensure that you have the following libraries installed: GMP (for arbitrary-precision arithmetic), OpenSSL (for cryptographic functions), and Boost (for various C++ utilities)

```
$ sudo apt-get update
$ sudo apt-get install -y libgmp-dev libssl-dev libboost-all-dev
```

### Install OTExtension
(Move to a partition with sufficient storage space.)
```
$ git clone https://github.com/encryptogroup/OTExtension.git
$ cd OTExtension
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

## Code
### Step 1: Write CMakeLists.txt
Use the `CMakeLists.txt` file for your project (modify appropriate information if needed). 

### Step 2: Write your code
See `main.cpp` file. 

### Step 3: Build your code
```
$ mkdir build && cd build
$ cmake ..
$ make
```

### Step 4: Run your code
```
$ 
```
```
$ 
```