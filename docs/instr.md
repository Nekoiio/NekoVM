# Key
| Short | Full | 
| :-- | :-- 
| R  | Register
| V  | Value (1 byte)
| A  | Address (2 bytes)
| P  | Placeholder
| __ | No value
---
| instruction | bytes | Example  | bytes |
| :-- | :-- | :-- | :--
| mov R, V | 00 RR VV | mov r0, 0x12 | 00 00 12
| mov   R, R | 09 RR RR | mov r0, 0x12 | 09 00 12
| add   R, R | 01 RR RR | add r0, r1   | 01 00 01
| add   R, V | 02 RR VV | add r0, 0x12 | 02 00 12
| sub   R, R | 03 RR RR | sub r0, r1   | 03 00 01
| sub   R, V | 04 RR VV | sub r0, 0x12 | 04 00 12
| cmp   R, R | 05 RR RR | cmp r0, r1   | 05 00 01
| cmo   R, V | 06 RR VV | cmp r0, 0x12 | 06 00 12
| jmp   A    | 07 AAAA  | jmp 0x4381   | 07 81 43
| jz    A    | 08 AAAA  | jz  0x4381   | 08 81 43
| push  R    | 10 RR    | push r0      | 10 00 __
| push  V    | 11 VV    | push 0x11    | 11 11 __
| push  A    | 12 AAAA  | push 0x1234  | 12 12 34
| pop   R    | 13 RR    | pop  r0      | 13 00 __
| pop16 R    | 14 RRRR  | pop  x0      | 14 10 __
| call  A    | 15 AAAA  | call 0x1234  | 15 12 34
| ret        | 16       | ret          | 16 __ __
| stp        | FF       | stp          | FF __ __
| P | P | P | P
| P | P | P | P
| P | P | P | P
| P | P | P | P



| 

| YX | 0 | 1 | 2 | 3 |
| :--- | :---: | :---: | :---: | :---: |
| **0** | r0 | r1 | r2 | r3 
| **1** | x0 | x1 | _  | _