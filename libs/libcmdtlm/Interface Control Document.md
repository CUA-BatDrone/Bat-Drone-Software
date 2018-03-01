# Endianness
all values are in **big endian**

# Command / Telemetry Packet Structure

| Offset | Length | Type    | Name |Description                     |
| ------ | ------ | ------- | ---- | ------------------------------ |
| 0      | 1      | uint_8  | type | 1 for command, 0 for telemetry |
| 1      | 1      | uint_8  | id   | Command / Telemetry ID         |
| 2      |        |         |      | Command / Telemetry Data       |

The length of the command / telemetry data depends on the command.

# Commands

| Command ID | Name         | Description |
| ---------- | ------------ | ----------- |
| 0          | noop         | Does nothing |
| 1          | direction    | Command to change direction |

# Telemetry

| Telemetry ID | Name          | Description |
| ------------ | ------------- | ----------- |
| 0            | echo          | Responds to echo_request |
