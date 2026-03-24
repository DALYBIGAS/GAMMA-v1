# Simulator-v1

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

## ⚙️ Dependencies

According to the official workflow, the following components are required:

### 1. LLVM + MLIR

```bash
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
git checkout 339a7687e1c036a5f91c9d5391523b93e2e76cd3
```
