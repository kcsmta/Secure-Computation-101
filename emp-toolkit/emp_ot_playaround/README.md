## Follow this [official repo](https://github.com/emp-toolkit/emp-ot).


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
$ python install.py --install --tool --ot 
```
After running above commands, you can see the folders `emp-ot` and `emp-tool` inside the folder `/usr/local/include/`, and `emp` library in `/user/local/lib`. Once the `emp-ot` installed, you can use the `emp-ot` in your code.

## Code
### Step 1: Write CMakeLists.txt
Use the `CMakeLists.txt` file for your project (modify appropriate information if needed).

### Step 2: Write your code
See files `main.cpp`. Note that `emp` (pre)defines constants `ALICE=1` and `BOB=2` (you can see it in `/usr/local/include/emp-tool/utils/constants.h`).

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