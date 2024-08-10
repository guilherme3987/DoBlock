```
Configure o JSON corretamente para não ser necessário importar arquivos .c no cabeçalho.
Exemplo: 
```
project/
├── include/
│ ├── AnaSintDoBlock.h
│ ├── FuncAuxDoBlock.h
│ ├── Processador_tokens.h
├── src/
│ ├── AnaSintDoBlock.c
│ ├── FuncAuxDoBlock.c
│ ├── Processador_tokens.c
│ ├── main.c
├── .vscode/
│ ├── tasks.json
├── README.md


```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "gcc",
            "args": [
                "-Wall",
                "-Wextra",
                "-Iinclude",
                "-o",
                "${workspaceFolder}/myprogram",
                "${workspaceFolder}/src/main.c",
                "${workspaceFolder}/src/AnaSintDoBlock.c",
                "${workspaceFolder}/src/FuncAuxDoBlock.c",
                "${workspaceFolder}/src/Processador_tokens.c"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        },
        {
            "label": "clean",
            "type": "shell",
            "command": "rm",
            "args": [
                "-f",
                "${workspaceFolder}/myprogram"
            ],
            "group": "build",
            "problemMatcher": []
        }
    ]
} 
