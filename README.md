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

### 1. LLVM + MLIR

```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout 339a7687e1c036a5f91c9d5391523b93e2e76cd3
```

### 2. SODA-OPT

```bash
git clone https://github.com/pnnl/soda-opt.git
cd soda-opt
```

Apply patch and build:

```bash
git apply arm_func.patch
cd build_tools
./build_llvm.sh <path/to/llvm/src> <llvm_build_dir> <llvm_install_dir>
./build_tools/build_soda.sh <source_dir> <install_dir> <build_dir>
<path/to/llvm/build/dir> <path/to/llvm/install/dir>
```

### 3. Torch-MLIR + PyTorch

```bash
# Create a virtual environment in the specified directory (optional)
python -m venv /path/to/torch-mlir/virtual/environment
# Used to enter the virtual environment
source /path/to/torch-mlir/virtual/environment/bin/activate

# Download the specified version of torch and torch-mlir
get https://github.com/llvm/torch-mlir/releases/download/snapshot20230101.76/torch-2.0.0.dev20230101+cpu-cp310-cp310-linux_x86_64.whl
wget https://github.com/llvm/torch-mlir/releases/download/snapshot20230101.76/torch_mlir-20230101.76-cp310-cp310-linux_x86_64.whl

# pip install
pip install torch-2.0.0.dev20230101+cpu-cp310-cp310-linux_x86_64.whl
pip install torch_mlir-20230101.76-cp310-cp310-linux_x86_64.whl
pip install transformers
pip install pydot
```

### 4. gem5-SALAM

```bash
git clone https://github.com/TeCSAR-UNCC/gem5-SALAM.git
sudo apt install build-essential git m4 scons zlib1g zlib1g-dev \
 libprotobuf-dev protobuf-compiler libprotoc-dev libgoogle-perftools-dev \
 python3-dev python-is-python3 libboost-all-dev pkg-config

#The framework can be built in either optimization (opt) or debugging (debug) mode, contingent upon the requirement for debugging capabilities, such as utilizing GDB to debug the gem5 simulator or its executing workloads.
cd gem5-SALAM
#opt mode
scons build/ARM/gem5.opt -j`nproc`
#debug mode
scons build/ARM/gem5.debug -j`nproc`
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
