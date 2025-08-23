#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

class InstructionDecoderTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();
    }

    std::unique_ptr<Cpu> cpu;
};

TEST_F(InstructionDecoderTest, DecodeRTypeAddInstruction)
{
    // Test decoding: add $t0, $t0, $t1 (0x01094020)
    // Opcode: 0, rs: 8 ($t0), rt: 9 ($t1), rd: 8 ($t0), function: 32 (add)
    uint32_t word = 0x01094020;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "add");

    // Test execution
    cpu->getRegisterFile().write(8, 5); // $t0 = 5
    cpu->getRegisterFile().write(9, 3); // $t1 = 3

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 8); // $t0 should be 5 + 3 = 8
}

TEST_F(InstructionDecoderTest, DecodeRTypeSubInstruction)
{
    // Test decoding: sub $t2, $t0, $t1 (0x01095022)
    // Opcode: 0, rs: 8 ($t0), rt: 9 ($t1), rd: 10 ($t2), function: 34 (sub)
    uint32_t word = 0x01095022;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "sub");

    // Test execution
    cpu->getRegisterFile().write(8, 7); // $t0 = 7
    cpu->getRegisterFile().write(9, 3); // $t1 = 3

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(10), 4); // $t2 should be 7 - 3 = 4
}

TEST_F(InstructionDecoderTest, DecodeITypeAddiInstruction)
{
    // Test decoding: addi $t1, $t0, 10 (0x2109000A)
    // Opcode: 8 (addi), rs: 8 ($t0), rt: 9 ($t1), immediate: 10
    uint32_t word = 0x2109000A;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "addi");

    // Test execution
    cpu->getRegisterFile().write(8, 5); // $t0 = 5

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(9), 15); // $t1 should be 5 + 10 = 15
}

TEST_F(InstructionDecoderTest, DecodeITypeLwInstruction)
{
    // Test decoding: lw $t0, 4($zero) (0x8C080004)
    // Opcode: 35 (lw), rs: 0 ($zero), rt: 8 ($t0), immediate: 4
    uint32_t word = 0x8C080004;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "lw");

    // Test execution
    cpu->getMemory().writeWord(4, 0xDEADBEEF);

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(8), 0xDEADBEEF); // $t0 should contain loaded value
}

TEST_F(InstructionDecoderTest, DecodeITypeSwInstruction)
{
    // Test decoding: sw $t1, 8($zero) (0xAC090008)
    // Opcode: 43 (sw), rs: 0 ($zero), rt: 9 ($t1), immediate: 8
    uint32_t word = 0xAC090008;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "sw");

    // Test execution
    cpu->getRegisterFile().write(9, 0xCAFEBABE); // $t1 = test value

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getMemory().readWord(8), 0xCAFEBABE); // Memory should contain stored value
}

TEST_F(InstructionDecoderTest, DecodeJTypeJInstruction)
{
    // Test decoding: j 0x40 (0x08000010)
    // Opcode: 2 (j), jump target: 0x40
    uint32_t word = 0x08000010;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "j");

    // Note: Jump instruction execution would require label mapping
    // which is not easily testable in isolation
}

TEST_F(InstructionDecoderTest, DecodeSyscallInstruction)
{
    // Test decoding: syscall (0x0000000C)
    // Opcode: 0, function: 12 (syscall)
    uint32_t word = 0x0000000C;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "syscall");

    // Test syscall execution
    cpu->getRegisterFile().write(2, 1);  // $v0 = 1 (print_int)
    cpu->getRegisterFile().write(4, 42); // $a0 = 42

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getConsoleOutput(), "42");
}

TEST_F(InstructionDecoderTest, DecodeNegativeImmediate)
{
    // Test decoding: addi $t1, $t0, -1 (0x2109FFFF)
    // Opcode: 8 (addi), rs: 8 ($t0), rt: 9 ($t1), immediate: -1 (0xFFFF)
    uint32_t word = 0x2109FFFF;

    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);
    EXPECT_EQ(instruction->getName(), "addi");

    // Test execution
    cpu->getRegisterFile().write(8, 5); // $t0 = 5

    instruction->execute(*cpu);
    EXPECT_EQ(cpu->getRegisterFile().read(9), 4); // $t1 should be 5 + (-1) = 4
}

TEST_F(InstructionDecoderTest, DecodeUnknownInstruction)
{
    // Test decoding unknown instruction
    uint32_t word = 0xFFFFFFFF; // Invalid instruction

    auto instruction = InstructionDecoder::decode(word);
    EXPECT_EQ(instruction, nullptr); // Should return nullptr for unknown instructions
}

TEST_F(InstructionDecoderTest, DecodeFieldExtraction)
{
    // Test field extraction manually for verification
    uint32_t word = 0x01094020; // add $t0, $t0, $t1

    // Use a static method access pattern (though these are private)
    // We'll test through the decode function instead
    auto instruction = InstructionDecoder::decode(word);
    ASSERT_NE(instruction, nullptr);

    // Verify it's the correct instruction type
    EXPECT_EQ(instruction->getName(), "add");
}

TEST_F(InstructionDecoderTest, DecodeComplexProgram)
{
    // Test decoding a sequence of instructions
    std::vector<uint32_t> program = {
        0x20080005, // addi $t0, $zero, 5
        0x20090003, // addi $t1, $zero, 3
        0x01095020, // add $t2, $t0, $t1
        0x0000000C  // syscall
    };

    std::vector<std::unique_ptr<Instruction>> instructions;

    for (uint32_t word : program)
    {
        auto instruction = InstructionDecoder::decode(word);
        ASSERT_NE(instruction, nullptr);
        instructions.push_back(std::move(instruction));
    }

    // Verify instruction types
    EXPECT_EQ(instructions[0]->getName(), "addi");
    EXPECT_EQ(instructions[1]->getName(), "addi");
    EXPECT_EQ(instructions[2]->getName(), "add");
    EXPECT_EQ(instructions[3]->getName(), "syscall");

    // Test execution of the program
    cpu->getRegisterFile().write(2, 1);  // $v0 = 1 (print_int) for syscall
    cpu->getRegisterFile().write(4, 10); // $a0 = 10 (value to print)

    for (auto& instr : instructions)
    {
        instr->execute(*cpu);
        if (cpu->shouldTerminate())
            break;
    }

    EXPECT_EQ(cpu->getRegisterFile().read(8), 5);  // $t0 = 5
    EXPECT_EQ(cpu->getRegisterFile().read(9), 3);  // $t1 = 3
    EXPECT_EQ(cpu->getRegisterFile().read(10), 8); // $t2 = 8 (5+3)
    EXPECT_EQ(cpu->getConsoleOutput(), "10");      // syscall output
}
