## Follow this [official repo](https://github.com/encryptogroup/ABY).


## Installation
It is recommended to download and build the library in a partition with sufficient storage space.

### Install dependencies
The ABY library relies on several dependencies for arithmetic computation, cryptographic functions, and other utilities. Ensure that you have the following libraries installed: GMP (for arbitrary-precision arithmetic), OpenSSL (for cryptographic functions), and Boost (for various C++ utilities)

```
$ sudo apt-get update
$ sudo apt-get install -y libgmp-dev libssl-dev libboost-all-dev
```

- [Relic](https://github.com/relic-toolkit/relic)
- [ENCRYPTO_utils](https://github.com/encryptogroup/ENCRYPTO_utils)
- [OTExtension](https://github.com/encryptogroup/OTExtension)

### Install ABY
(Move to a partition with sufficient storage space.)
```
$ git clone https://github.com/encryptogroup/ABY.git
$ cd ABY
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

*Note 1: in my case, I had to manually add `#include <cstdlib>` in the header file `ABY/extern/ENCRYPTO_utils/src/ENCRYPTO_utils/channel.h`. Otherwise, it raised an error related to `malloc` and `free` functions. Should we install `OTExtension` and `ENCRYPTO_utils` libraries before installing `ABY`?*

*Note 2: in my case, I had to manually comment `find_dependency(MIRACL)` in the file `/usr/local/lib/cmake/ABY/ABYConfig.cmake`. According to [this issue](https://github.com/encryptogroup/ABY/issues/151), the authors replaced `MIRACL` as dependency with the `RELIC` library*

After running above commands, you can see the folders `abycore`, `ENCRYPTO_utils`, `ot`, and `relic` inside the folder `/usr/local/include/`, and `libaby.a`, `libencrypto_utils.a`, `libotextention.a`, and `librelic_s.a` library in `/user/local/lib`. Once the `ABY` installed, you can use the `ABY` in your code.

## Code
### Step 1: Write CMakeLists.txt
Use the `CMakeLists.txt` file for your project (modify appropriate information if needed). In `add_executable` in the `CMakeLists.txt`, change the file you want to build.

### Step 2: Write your code
See `.cpp` files such as `millionaire.cpp`, `bit.cpp`, `bit.cpp`,... 

### Step 3: Build your code
```
$ mkdir build && cd build
$ cmake ..
$ make
```

### Step 4: Run your code
Alice (sender):
```
$ ./emp_ot_playground 0
```
Bob (receiver)
```
$ ./emp_ot_playground 1
```