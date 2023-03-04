# Notes LibCLI Library

Describe CLI, Why LibCLI Library, and implementing new handlers. 

## Why LIBCLI

Menu based do not scale when multiple commads in
multiple levels. 
The commandparser folder contain LIBCLI.

## TestAPP for Mock a CLI app

- ```cd CommandParser```

- ```make -f MakeFile``` or ```make```

## How to use CLI

CLI i built using a treestructure under root, first level of para called hooks. 
Other levels called leafs.

- All hooks: ```?```

- Full complete commands: ```.```

- All show hooks: ```show ?```

- Complete show commands: ```show .```

- help: ```show help```

- exit: ```run term```

## Implement new CMD Handlers

__Handler are built__
- Part 1: Extract command code

- Part 2 Iterate over leaf value 

- Part 3: Switch implment the logic using leaf values