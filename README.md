# GAMMA-v1

A simulation framework based on **gem5 + MLIR + custom accelerator modeling**, designed for exploring hardware-software co-design, accelerator mapping, and system-level simulation.

---

## 📌 Overview

This project integrates:

- **PyTorch / Torch-MLIR** (frontend model description)
- **MLIR + SODA-OPT** (IR transformation & optimization)
- **gem5-SALAM** (cycle-accurate simulation)
- **Custom accelerator modeling** (e.g., systolic array, matmul kernel)

### 🔄 Workflow Capabilities

- Neural network → MLIR → LLVM IR
- Accelerator extraction & mapping
- System-level simulation in gem5

---

## 📂 Project Structure

```bash
Simulator/
├── benchmarks/
│   └── MiniLM/
├── hw/
├── sw/
├── config.yaml
├── main.cpp
├── top.cpp
└── script.py
```




## ⚙️ Dependencies

According to the official workflow, the following components are required:
### 0. Building from Source
Use git to clone this repo.
```bash
git clone https://github.com/DALYBIGAS/GAMMA-v1.git
```
Checkout the develop branch. Currently only this branch contains main content of project
```bash
cd GAMMA-v1
git checkout develop
```
Initialize and update the submodules llvm-project, soda-opt and gem5-SALAM.
```bash
git submodule init
git submodule update
```
### 1. LLVM + MLIR
The environment may resolve the dependencies and meet the requirement by following the instructions in the [LLVM Getting Started page](https://llvm.org/docs/GettingStarted.html#requirements). In Ubuntu 22.04, you may follow these commands to install the dependencies.

```bash
sudo apt-get install build-essential cmake ninja-build python3-pip

# Using pip as package manager
# You may would like to install the packages in a virtual environment
pip install pybind11 numpy
```

### 2. SODA-OPT
Build LLVM and MLIR from SODA-OPT project. You may refer to the [How to build](https://github.com/pnnl/soda-opt) section in its README file. The steps using Helper Script are listed below.

```bash
git clone https://github.com/pnnl/soda-opt.git
cd soda-opt
```

Apply patch and build:

```bash
git apply arm_func.patch
cd build_tools
#To configure, build, and install
./build_llvm.sh <path/to/llvm/src> <llvm_build_dir> <llvm_install_dir>
# To configure, build, and install
./build_tools/build_soda.sh <source_dir> <install_dir> <build_dir>
<path/to/llvm/build/dir> <path/to/llvm/install/dir>
```
After building LLVM, MLIR and SODA-OPT, you may need to add the path to their binaries to the environment variable PATH.
```bash
export PATH=$PATH:<path/to/llvm/install/bin>:<path/to/soda-opt/install/bin>
```

### 3. Torch-MLIR + PyTorch
Download the wheel package of torch and torch-mlir from the [release page](https://github.com/llvm/torch-mlir/releases).So far we tested following specific versions:
```bash
# torch
wget https://github.com/llvm/torch-mlir/releases/download/oneshot-20230101.76/torch-2.0.0.dev20230101+cpu-cp310-cp310-linux_x86_64.whl

# torch-mlir
wget https://github.com/llvm/torch-mlir/releases/download/oneshot-20230101.76/torch_mlir-20230101.76-cp310-cp310-linux_x86_64.whl
```
Install the wheel packages using pip.
```bash
pip install torch-2.0.0.dev20230101+cpu-cp310-cp310-linux_x86_64.whl
pip install torch_mlir-20230101.76-cp310-cp310-linux_x86_64.whl
```
### 4. gem5-SALAM
You may refer to the [Building gem5](https://www.gem5.org/documentation/general_docs/building) to install dependencies on different OS.


For Ubuntu 22.04, you may install the dependencies using the following commands.
```bash
git clone https://github.com/TeCSAR-UNCC/gem5-SALAM.git
sudo apt install build-essential git m4 scons zlib1g zlib1g-dev \
    libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev \
    python3-dev libboost-all-dev pkg-config python3-tk
```
Then build the gem5-SALAM project.
```bash
cd gem5-SALAM
scons build/ARM/gem5.opt -j`nproc`
```
### Transformers (Optional)
You may not need this package if you are not going to use the transformer model. You can install other packages like openCV if you want to use related models.
```bash
pip install transformers
```
Install the transformers package using pip.
### pydot and graphviz (Optional)
pydot and graphviz are used to visualize the system config file in gem5. Install the pydot and graphviz packages using pip.
```bash
pip install pydot graphviz
```
## 🚀 Workflow

The complete simulation pipeline includes:

### Step 1: Model Definition (PyTorch)

Define your model (e.g., MiniLM):

```bash
linalg_on_tensors_mlir = torch_mlir.compile(
    model,
    example_input,
    output_type="LINALG_ON_TENSORS",
    use_tracing=True
)
```

### Step 2: MLIR Optimization (PASS)

```bash
mlir-opt \
 -linalg-fuse-elementwise-ops \
 -canonicalize \
 -convert-tensor-to-linalg \
 ...
```

STiling optimization:

```bash
soda-opt input.mlir \
 -soda-linalg-tile="tile-sizes=... anchor-op=linalg.batch_matmul"

```

### Step 3: Host Code Generation

```bash
mlir-translate --mlir-to-llvmir input.mlir > output.ll
```

### Step 4: Accelerator Mapping

* Extract accelerator functions
* Generate runtime (`main.cpp`)
* Generate hardware controller (`top.cpp`)
* Generate system config (`config.yaml`)

```bash
python generateCustomizedRuntime.py ...
python generateTopAccel.py ...
python generateConfigYAML.py ...
```

### Step 5: Accelerator Implementation

Example (matrix multiplication kernel):

```bash
#pragma unroll
for(int k=0;k<384;k++) {
    mult = m1[i_col + k] * m2[k_col + j];
    sum += mult;
}
```

### Step 6: Simulation

runs full system simulation in gem5：

```bash
bash ${M5_PATH}/tools/run_system.sh \
--bench MiniLM \
--bench-path benchmarks/MiniLM
```

## 📊 Outputs

After simulation:

* `stats.txt` → performance statistics
* `system.terminal` → runtime output
* `config.dot.pdf` → system architecture graph

## 🧠 Key Features

* MLIR-based accelerator extraction
* Flexible hardware mapping (tiling, fusion)
* Custom accelerator modeling
* Full-system simulation (CPU + accelerator)
* Extensible to Transformer / CNN workloads
