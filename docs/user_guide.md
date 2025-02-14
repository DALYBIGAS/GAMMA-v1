# User Guide

## MLIR Code Generation

<!-- ### Implementation of Benchmark Using PyTorch  -->

Enter the `benchmarks` directory.

```bash
cd benchmark
```

Copy the `template` project to a new directory and rename it.

```bash
cp -r template_project new_project
```

Edit the template.py to define the model and input according to the benchmark you want to implement and the commented code.

An example of the template.py is shown below implementing MiniLM algorithm.

```python
# -----------------------------------

# Please provide <model> and <example_input> variable definitions

# -----------------------------------

import warnings
warnings.simplefilter("ignore")
import os
os.environ["TOKENIZERS_PARALLELISM"] = "true"
from transformers import AutoTokenizer, AutoModelForSequenceClassification

# Specify the model name to be pulled from the HuggingFace model hub
model_name = "philschmid/MiniLM-L6-H384-uncased-sst2"

# -----------------------------------

# The following is the definition of the example_input variable

# Specify the Tokenizer to be extracted from the model
def prepare_sentence_tokens(hf_model: str, sentence: str):

    tokenizer = AutoTokenizer.from_pretrained(hf_model)
    # print(tokenizer)

    return torch.tensor([tokenizer.encode(sentence)])

# Provide a sentence as an input test sample
sentence = "The quick brown fox jumps over the lazy dog."

# Process the sentence using the Tokenizer captured from the model to obtain the model input
example_input = prepare_sentence_tokens(model_name, sentence)

# -----------------------------------

# The following is the definition of the model variable

# Adjust model features and parameters through a wrapper
class OnlyLogitsHuggingFaceModel(torch.nn.Module):

    def __init__(self, model_name: str):
        super().__init__()
        self.model = AutoModelForSequenceClassification.from_pretrained(
            # Pre-trained model name
            model_name, 
            # Number of output labels
            num_labels=2,
            # Other configurations
            output_attentions=False,
            output_hidden_states=False,
            torchscript=True,
        )
        self.model.eval()

    def forward(self, input):
        # Return only the logits.
        return self.model(input)[0]

model = OnlyLogitsHuggingFaceModel(model_name)
# print(model)

# The definitions of the two required variables are now complete

# -----------------------------------

with open('example_input.log', 'w', encoding='utf-8') as file:
    # Write the contents of the variable to a file
    file.write(str(example_input))

linalg_on_tensors_mlir = torch_mlir.compile(
    model,
    example_input,
    output_type="LINALG_ON_TENSORS",
    use_tracing=True)

mlir_file_path = '02-linalg-on-tensors.mlir'
with open(mlir_file_path, 'wt') as f:
    print(linalg_on_tensors_mlir.operation.get_asm(), file=f)

result = model.forward(example_input)
```

Now you get the MLIR code named `02-linalg-on-tensors.mlir` in the `new_project` directory.

## Hardware Configuration

