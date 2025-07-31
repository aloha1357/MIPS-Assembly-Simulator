#include <gtest/gtest.h>
#include "Cpu.h"
#include "Memory.h"
#include "RegisterFile.h"
#include <memory>

namespace mips {

class SyscallDebugTest : public ::testing::Test {
protected:
    void SetUp() override {
        cpu = std::make_unique<Cpu>();
    }
    
    std::unique_ptr<Cpu> cpu;
};

// Test direct memory reading with string  
TEST_F(SyscallDebugTest, DetailedStringMemoryTest) {
    auto& memory = cpu->getMemory();
    
    // Test 1: Store simple "Hi" string
    uint32_t hiString = 0x6948; // H=0x48 at byte 0, i=0x69 at byte 1, rest zeros
    memory.writeWord(0x1000, hiString);
    
    // Read back and verify
    uint32_t stored = memory.readWord(0x1000);
    EXPECT_EQ(stored, 0x6948) << "Memory storage should work";
    
    // Test manual byte extraction (same as syscall)
    std::string extractedString;
    char byte0 = (stored >> 0) & 0xFF;   // Should be 'H' = 0x48
    char byte1 = (stored >> 8) & 0xFF;   // Should be 'i' = 0x69
    char byte2 = (stored >> 16) & 0xFF;  // Should be 0x00
    char byte3 = (stored >> 24) & 0xFF;  // Should be 0x00
    
    std::cout << "Extracted bytes: " 
              << std::hex << (int)(unsigned char)byte0 << " "
              << std::hex << (int)(unsigned char)byte1 << " "
              << std::hex << (int)(unsigned char)byte2 << " "
              << std::hex << (int)(unsigned char)byte3 << std::endl;
    
    // Build string same way as syscall
    if (byte0 != '\0') extractedString += byte0;
    if (byte1 != '\0') extractedString += byte1;
    
    EXPECT_EQ(extractedString, "Hi") << "Manual extraction should produce 'Hi'";
    
    // Test 2: Try single character string
    memory.writeWord(0x2000, 0x48); // Just 'H'
    uint32_t singleChar = memory.readWord(0x2000);
    char sc = singleChar & 0xFF;
    EXPECT_EQ(sc, 'H') << "Single character should work";
}

// Test syscall 4 behavior directly  
TEST_F(SyscallDebugTest, DirectSyscall4Test) {
    // Set up memory with simple string
    auto& memory = cpu->getMemory();
    memory.writeWord(0x1000, 0x48); // Just 'H' + null terminator
    
    // Set up registers for syscall 4
    cpu->getRegisterFile().write(2, 4);    // $v0 = 4 (print_string)
    cpu->getRegisterFile().write(4, 0x1000); // $a0 = string address
    
    // Clear console output
    std::string initialOutput = cpu->getConsoleOutput();
    
    // Create and execute syscall instruction manually
    // This simulates what happens in the pipeline
    auto& registerFile = cpu->getRegisterFile();
    uint32_t syscallNumber = registerFile.read(2);
    EXPECT_EQ(syscallNumber, 4) << "Syscall number should be 4";
    
    uint32_t stringAddress = registerFile.read(4);
    EXPECT_EQ(stringAddress, 0x1000) << "String address should be 0x1000";
    
    // Manually execute the string printing logic
    std::string str;
    uint32_t currentAddr = stringAddress;
    uint32_t word = memory.readWord(currentAddr);
    
    std::cout << "Read word from memory: 0x" << std::hex << word << std::endl;
    
    for (int i = 0; i < 4; i++) {
        char c = (word >> (i * 8)) & 0xFF;
        std::cout << "Byte " << i << ": 0x" << std::hex << (int)(unsigned char)c 
                  << " ('" << c << "')" << std::endl;
        if (c == '\0') {
            break;
        }
        str += c;
    }
    
    std::cout << "Extracted string: '" << str << "'" << std::endl;
    EXPECT_EQ(str, "H") << "Should extract 'H'";
}

} // namespace mips
