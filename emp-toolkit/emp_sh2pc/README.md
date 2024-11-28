## Follow this [official repo](https://github.com/emp-toolkit/emp-sh2pc).


## Installation
It is recommended to download and build the library in a partition with sufficient storage space.

### Install dependencies
The EMP toolkit relies on these libraries for cryptographic functions like EC_POINT (Elliptic Curve), BN_CTX (BigNum Context), and various hash functions.

- Install the OpenSSL development package
```
$ sudo apt-get update
$ sudo apt-get install libssl-dev
```

### Install EMP
```
$ wget https://raw.githubusercontent.com/emp-toolkit/emp-readme/master/scripts/install.py
$ python install.py --install --tool --ot --sh2pc
```
After running above commands, you can see the folders `emp-sh2mp` and `emp-tool` inside the folder `/usr/local/include/`, and `libemp-tool.so` library in `/user/local/lib`. Once the `emp-sh2mp` installed, you can use the `emp-sh2mp` in your code.

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