# Key
| Short | Full | 
| :-- | :-- 
| R | Register
| V | Value
| A | Address
| P | Placeholder
---
| instruction | bytes | Example  | bytes |
| :-- | :-- | :-- | :--
| MOV R, V | 00 RR VV | MOV r0, 0x12 | 00 00 12
| MOV R, R | 09 RR RR | MOV r0, 0x12 | 09 00 12
| ADD R, R | 01 RR RR | ADD r0, r1   | 01 00 01
| ADD R, V | 02 RR VV | ADD r0, 0x12 | 02 00 12
| SUB R, R | 03 RR RR | SUB r0, r1   | 03 00 01
| SUB R, V | 04 RR VV | SUB r0, 0x12 | 04 00 12
| CMP R, R | 05 RR RR | CMP r0, r1   | 05 00 01
| CMP R, V | 06 RR VV | CMP r0, 0x12 | 06 00 12
| JMP A    | 07 AAAA  | JMP 0x4381   | 07 81 43
| JZ A     | 08 AAAA  | JMP 0x4381   | 08 81 43
| STP | FF | STP | FF 00 00 
| P | P | P | P
| P | P | P | P
| P | P | P | P
| P | P | P | P
| P | P | P | P
| P | P | P | P
| P | P | P | P

