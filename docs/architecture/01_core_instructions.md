# language: en

# Spec by Example (SBE) – MIPS Core Instructions

# ------------------------------------------------

# Each Scenario is an executable example that illustrates ONE concrete

# behaviour of the simulator.  These examples **are** the requirements.

Feature: Core arithmetic and memory instructions The simulator must reproduce the observable behaviour of the baseline R‑/I‑type instruction subset.

Background: Given an empty RegisterFile initialised to zeros And an empty Data Memory initialised to zeros

# ────────────────────────────────────────────────

# R‑type arithmetic

# ────────────────────────────────────────────────

Scenario Outline: add writes the sum of two registers into rd Given register  contains \<rs\_val> And   register  contains \<rt\_val> When  the program """ add , ,  """ is executed for 1 cycle Then  register  should equal&#x20;

```
Examples:
  | rs | rt | rd | rs_val | rt_val | expected |
  | $t0| $t1| $t2|   3    |   5    |    8     |
  | $s0| $s1| $s2|  -1    |   1    |    0     |
  | $a0| $a1| $v0|   0    |   0    |    0     |
```

Scenario Outline: sub subtracts rt from rs and writes into rd Given register  contains \<rs\_val> And   register  contains \<rt\_val> When  the program """ sub , ,  """ is executed for 1 cycle Then  register  should equal&#x20;

```
Examples:
  | rs | rt | rd | rs_val | rt_val | expected |
  | $t0| $t1| $t2|   5    |   3    |    2     |
  | $s0| $s1| $s2|   1    |   1    |    0     |
```

# ────────────────────────────────────────────────

# I‑type immediate arithmetic

# ────────────────────────────────────────────────

Scenario Outline: addi adds sign‑extended immediate to rs Given register  contains \<rs\_val> When  the program """ addi , ,  """ is executed for 1 cycle Then  register  should equal&#x20;

```
Examples:
  | rs | rt | imm | rs_val | expected |
  | $t0| $t1|  10 |    5   |    15    |
  | $s0| $s1| -4  |    3   |    -1    |
```

# ────────────────────────────────────────────────

# Memory access

# ────────────────────────────────────────────────

Scenario: lw loads word from memory into register Given data memory address 0x1000 contains value 0xDEADBEEF When  the program """ lw \$t0, 0x1000(\$zero) """ is executed for 2 cycles Then  register \$t0 should equal 0xDEADBEEF

Scenario: sw stores register value into memory Given register \$t1 contains 0xCAFEBABE When  the program """ sw \$t1, 0x2000(\$zero) """ is executed for 2 cycles Then  data memory address 0x2000 should equal 0xCAFEBABE

# ────────────────────────────────────────────────

# Control transfer

# ────────────────────────────────────────────────

Scenario: beq taken branch flushes delay slot and updates PC Given register \$t0 contains 1 And   register \$t1 contains 1 When  the program """ beq \$t0, \$t1, label addi \$v0, \$zero, 0      # delay slot (should be flushed) label: addi \$v0, \$zero, 42 """ is executed for 6 cycles Then  register \$v0 should equal 42

Scenario: j unconditional jump sets PC to target address Given register \$v0 contains 0 When  the program """ j target addi \$v0, \$zero, 0      # delay slot target: addi \$v0, \$zero, 7 """ is executed for 6 cycles Then  register \$v0 should equal 7
