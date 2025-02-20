# Develop Notes

## TODO Tasks

### Driver Generator Related

#### Config

 - [ ] YAML
 - [ ] Parser (Process_config)

#### Single accelerator

##### Basic components library: 

 - [x] DMA (Stream, Noncoherent, Tensor Transfer)
 - [x] Accelerator

#### Multiple accelerators

 - [ ] Op fusion
 - [ ] Pipelining
 - [ ] Double buffer

### Transform Generator Related

 - [ ] Op fusion
 - [ ] Pipelining
 - [ ] Double buffer

### Design Space Exploration

Add tasks here.

### Docs Related

 - [ ] Update the docs for the new features.

# Develop Refenrence

## MLIR Related

- MLIR test codes, which include many examples of how to use Transform dialect (named transform-xxxx.mlir in each dialect's folder). See [github repository](https://github.com/llvm/llvm-project/tree/main/mlir/test).
- Transform dialect's documentation. See [Transform dialect](https://mlir.llvm.org/docs/Dialects/Transform/).


