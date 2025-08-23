#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/InstructionDecoder.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>

using namespace mips;

/**
 * Character Syscall Tests - Following TDD approach
 */
class CharacterSyscallTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        memory       = std::make_unique<Memory>();
        registerFile = std::make_unique<RegisterFile>();
        cpu          = std::make_unique<Cpu>();
    }

    std::unique_ptr<Memory>       memory;
    std::unique_ptr<RegisterFile> registerFile;
    std::unique_ptr<Cpu>          cpu;
};

/**
 * BDD: Given syscall number 11 and character code in $a0
 *      When syscall is executed
 *      Then character should be printed to stdout
 */
TEST_F(CharacterSyscallTest, Given_PrintCharSyscall_When_ExecutedWithA_Then_APrinted)
{
    // Given: $v0 = 11 (print_character), $a0 = 65 ('A')
    cpu->getRegisterFile().write(2, 11);  // $v0 = print_character
    cpu->getRegisterFile().write(4, 65);  // $a0 = 'A'

    // When: Execute syscall
    SyscallInstruction syscall;
    syscall.execute(*cpu);

    // Then: 'A' should be in console output
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find('A') != std::string::npos);
}

TEST_F(CharacterSyscallTest, Given_PrintCharSyscall_When_ExecutedWithNewline_Then_NewlinePrinted)
{
    // Given: $v0 = 11 (print_character), $a0 = 10 ('\n')
    cpu->getRegisterFile().write(2, 11);  // $v0 = print_character
    cpu->getRegisterFile().write(4, 10);  // $a0 = '\n'

    // When: Execute syscall
    SyscallInstruction syscall;
    syscall.execute(*cpu);

    // Then: Newline should be in console output
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find('\n') != std::string::npos);
}

TEST_F(CharacterSyscallTest,
       Given_PrintCharSyscall_When_ExecutedWithLargeValue_Then_OnlyLower8BitsUsed)
{
    // Given: $v0 = 11 (print_character), $a0 = 0x141 (321, should truncate to 65='A')
    cpu->getRegisterFile().write(2, 11);     // $v0 = print_character
    cpu->getRegisterFile().write(4, 0x141);  // $a0 = 321

    // When: Execute syscall
    SyscallInstruction syscall;
    syscall.execute(*cpu);

    // Then: Should print 'A' (lower 8 bits of 321 = 65)
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find('A') != std::string::npos);
}

/**
 * BDD: Given syscall number 12
 *      When syscall is executed with input available
 *      Then character should be read into $v0
 */
TEST_F(CharacterSyscallTest, Given_ReadCharSyscall_When_ExecutedWithInputB_Then_BReturnedInV0)
{
    // Given: $v0 = 12 (read_character), input contains 'B'
    cpu->getRegisterFile().write(2, 12);  // $v0 = read_character
    cpu->setConsoleInput("B");

    // When: Execute syscall
    SyscallInstruction syscall;
    syscall.execute(*cpu);

    // Then: $v0 should contain 'B' (66)
    EXPECT_EQ(66, cpu->getRegisterFile().read(2));
}

TEST_F(CharacterSyscallTest,
       Given_ReadCharSyscall_When_ExecutedWithMultipleChars_Then_FirstCharReturned)
{
    // Given: $v0 = 12 (read_character), input contains "Hello"
    cpu->getRegisterFile().write(2, 12);  // $v0 = read_character
    cpu->setConsoleInput("Hello");

    // When: Execute syscall twice
    SyscallInstruction syscall;
    syscall.execute(*cpu);
    uint32_t firstChar = cpu->getRegisterFile().read(2);

    cpu->getRegisterFile().write(2, 12);  // Reset for second call
    syscall.execute(*cpu);
    uint32_t secondChar = cpu->getRegisterFile().read(2);

    // Then: Should get 'H' then 'e'
    EXPECT_EQ(72, firstChar);    // 'H'
    EXPECT_EQ(101, secondChar);  // 'e'
}

TEST_F(CharacterSyscallTest, Given_ReadCharSyscall_When_NoInputAvailable_Then_EOFReturned)
{
    // Given: $v0 = 12 (read_character), no input
    cpu->getRegisterFile().write(2, 12);  // $v0 = read_character
    cpu->setConsoleInput("");             // Empty input

    // When: Execute syscall
    SyscallInstruction syscall;
    syscall.execute(*cpu);

    // Then: $v0 should contain EOF (-1, but as unsigned it's 0xFFFFFFFF)
    uint32_t result = cpu->getRegisterFile().read(2);
    EXPECT_EQ(0xFFFFFFFF, result);  // -1 as unsigned
}
