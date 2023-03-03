# Notes the CLI Program PartA NetWork in C

## Build executable

```make all -f <make_file>``, or
```make all```
```man make```

## Example commands

- Start CLI: ```tcstack.exe```
- Show help: ```show help```
- Show exit: ```run term```
- Show topo: ```show topo```
- Show route from topo: ```show node R1 rt``` 

## Topologies Creation

- Nodes are created in ```topoligies.c```

- Check files: ```graph.h/c```, ```net.h/c```

- MAC addressses created randomly.

## Topology Emulate/Simulate network commands

- ping: ```run node R1 ping <R2_ip_address>```

- arp: ```show node R1 arp```

## Logging emulated traffic

- __Enable logging all interfaces:__
ENTER setup: ```conf node R1 traceoptions flag /```
ask options: ```?```
show all option: ```all```
EXIT setup: ```cd```

- __Enable logging specific interface:__
ENTER setup: ```conf node R1 eth7 tr flag /```
ask options: ```?```
show all option: ```all```
EXIT setup: ```cd```

__Disable the setup logging:__
Disable: ```conf node R1 traceoptions flag all```

## Custom CLI Library LIBCLI

- LIBCLI parse CLI tokens
- LIBCLI validate CLI foramts
- LIBCLI invoke appropriate handler (e.g for L2,L3,L4 protocols)

## Libs the CLI use

- Linked-list
- Tress
- Timers
- CLIs
- Schedulers
- TLV encoders
- BIT manipulation MACROS

## TLV encoding (Type, Legth, Value)

__Type:__ 
A numeric code that identifies the type of data being encoded. For example, a type code might indicate whether the data represents a message header, a command, or a data payload.

__Length:__ 
The length of the data in bytes. This field specifies how many bytes follow the Length field to make up the data value.

__Value:__ 
The actual data being encoded. This field contains the bytes that make up the data value.
