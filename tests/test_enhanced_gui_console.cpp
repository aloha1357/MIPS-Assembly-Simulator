#include <gtest/gtest.h>
#include "gui/MipsSimulatorGUI.h"  // Use base implementation for testing
#include "Cpu.h"                   // Direct CPU testing
#include "Memory.h"                // Memory access
#include "Assembler.h"             // Direct assembler testing
#include <memory>
#include <string>

namespace mips {

/**
 * @brief Enhanced GUI Console test that addresses the testing environment mismatch
 * 
 * This test uses a hybrid approach:
 * 1. Tests console functionality through base GUI class (proven to work)
 * 2. Also tests CPU syscall functionality directly
 * 3. Provides more comprehensive validation of the console output pipeline
 */
class EnhancedGuiConsoleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create base GUI in headless mode 
        gui = std::make_unique<MipsSimulatorGUI>(true); // headless mode
        initialized = gui->initialize();
        
        // Also create direct CPU for syscall testing
        cpu = std::make_unique<Cpu>();
        assembler = std::make_unique<Assembler>();
    }
    
    void TearDown() override {
        if (gui && initialized) {
            gui->shutdown();
        }
    }
    
    std::unique_ptr<MipsSimulatorGUI> gui;
    std::unique_ptr<Cpu> cpu;
    std::unique_ptr<Assembler> assembler;
    bool initialized = false;
};

// Test 1: Direct CPU syscall testing - isolate the issue
TEST_F(EnhancedGuiConsoleTest, DirectCpuSyscallIntegerTest) {
    // Given: A CPU instance
    ASSERT_TRUE(cpu != nullptr) << "CPU should be created";
    
    // And: A simple program that prints an integer
    std::string program = R"(
addi $v0, $zero, 1      # syscall 1: print_int
addi $a0, $zero, 42     # value to print
syscall                 # Should print "42"

addi $v0, $zero, 10     # syscall 10: exit  
syscall
)";
    
    // When: We load and execute directly on CPU
    try {
        cpu->loadProgramFromString(program);
        
        // Execute until termination
        int maxCycles = 100;
        int cycles = 0;
        while (!cpu->shouldTerminate() && cycles < maxCycles) {
            cpu->tick();
            cycles++;
        }
        
        // Then: CPU console output should contain "42"
        std::string cpuOutput = cpu->getConsoleOutput();
        EXPECT_TRUE(cpuOutput.find("42") != std::string::npos) 
            << "CPU console output should contain '42' but was: '" << cpuOutput << "'";
            
    } catch (const std::exception& e) {
        FAIL() << "CPU execution failed: " << e.what();
    }
}

// Test 2: Direct CPU syscall testing for strings
TEST_F(EnhancedGuiConsoleTest, DirectCpuSyscallStringTest) {
    // Given: A CPU instance
    ASSERT_TRUE(cpu != nullptr) << "CPU should be created";
    
    // And: A program that creates and prints a string
    std::string program = R"(
# Create string "Hi" at address 0x1000
# Using little-endian byte order: H=0x48, i=0x69
addi $t0, $zero, 0x48       # 'H' = 0x48
addi $t1, $zero, 0x69       # 'i' = 0x69  
sll $t1, $t1, 8            # shift 'i' to byte position 1
add $t0, $t0, $t1          # combine H + i = 0x6948
sw $t0, 0x1000($zero)      # store "Hi\0\0" at address 0x1000

# Print the string
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # string address
syscall                    # Should print "Hi"

addi $v0, $zero, 10        # syscall 10: exit
syscall
)";
    
    // When: We execute the program directly
    try {
        cpu->loadProgramFromString(program);
        
        // Execute until termination  
        int maxCycles = 100;
        int cycles = 0;
        while (!cpu->shouldTerminate() && cycles < maxCycles) {
            cpu->tick();
            cycles++;
        }
        
        // Then: CPU console output should contain "Hi"
        std::string cpuOutput = cpu->getConsoleOutput();
        EXPECT_TRUE(cpuOutput.find("Hi") != std::string::npos) 
            << "CPU console output should contain 'Hi' but was: '" << cpuOutput << "' (hex dump: ";
        
        // Add hex dump for debugging only if test fails  
        if (cpuOutput.find("Hi") == std::string::npos) {
            std::cout << "Hex dump: ";
            for (char c : cpuOutput) {
                std::cout << std::hex << (int)(unsigned char)c << " ";
            }
            std::cout << std::endl;
        }
        
    } catch (const std::exception& e) {
        FAIL() << "CPU execution failed: " << e.what();
    }
}

// Test 3: Memory verification test
TEST_F(EnhancedGuiConsoleTest, MemoryStringStorageTest) {
    // Given: A CPU instance
    ASSERT_TRUE(cpu != nullptr) << "CPU should be created";
    
    // When: We manually store a string in memory and verify
    auto& memory = cpu->getMemory();
    
    // Store "Hi" manually: H=0x48, i=0x69, \0=0x00
    // Little-endian: 0x00006948
    uint32_t hiString = 0x6948; // H in lowest byte, i in next byte, rest zeros
    memory.writeWord(0x1000, hiString);
    
    // Verify memory content
    uint32_t storedValue = memory.readWord(0x1000);
    EXPECT_EQ(storedValue, 0x6948) << "Memory should store the string correctly";
    
    // Extract and verify individual bytes
    char byte0 = (storedValue >> 0) & 0xFF;  // Should be 'H' = 0x48
    char byte1 = (storedValue >> 8) & 0xFF;  // Should be 'i' = 0x69  
    char byte2 = (storedValue >> 16) & 0xFF; // Should be '\0' = 0x00
    
    EXPECT_EQ(byte0, 'H') << "First byte should be 'H'";
    EXPECT_EQ(byte1, 'i') << "Second byte should be 'i'";
    EXPECT_EQ(byte2, '\0') << "Third byte should be null terminator";
}

// Test 2: Integer output through actual syscall execution
TEST_F(EnhancedGuiConsoleTest, IntegerOutputThroughSyscall) {
    // Given: GUI simulator initialized
    ASSERT_TRUE(initialized) << "GUI should initialize successfully";
    
    // And: A simple program that prints an integer
    std::string program = R"(
# Simple integer printing test
addi $v0, $zero, 1      # syscall 1: print_int
addi $a0, $zero, 42     # value to print
syscall                 # Should print "42"

addi $v0, $zero, 10     # syscall 10: exit  
syscall
)";
    
    // When: We load and execute the program
    bool loaded = gui->loadProgram(program);
    EXPECT_TRUE(loaded) << "Program should load successfully";
    
    if (loaded) {
        gui->runProgram();
        
        // Then: Console should contain "42" from syscall output
        std::string consoleOutput = gui->getConsoleOutput();
        EXPECT_TRUE(consoleOutput.find("42") != std::string::npos) 
            << "Console output should contain '42' but was: '" << consoleOutput << "'";
    }
}

// Test 3: String output through actual syscall execution  
TEST_F(EnhancedGuiConsoleTest, StringOutputThroughSyscall) {
    // Given: GUI simulator initialized
    ASSERT_TRUE(initialized) << "GUI should initialize successfully";
    
    // And: A program that creates and prints a simple string
    std::string program = R"(
# Simple string printing test  
# Create string "Hi" at address 0x1000
addi $t0, $zero, 0x48       # 'H' = 0x48
addi $t1, $zero, 0x69       # 'i' = 0x69  
sll $t1, $t1, 8            # shift 'i' to byte position 1
add $t0, $t0, $t1          # combine H + i
sw $t0, 0x1000($zero)      # store "Hi\0\0" at address 0x1000

# Print the string
addi $v0, $zero, 4         # syscall 4: print_string
addi $a0, $zero, 0x1000    # string address
syscall                    # Should print "Hi"

addi $v0, $zero, 10        # syscall 10: exit
syscall
)";
    
    // When: We load and execute the program
    bool loaded = gui->loadProgram(program);
    EXPECT_TRUE(loaded) << "Program should load successfully";
    
    if (loaded) {
        gui->runProgram();
        
        // Then: Console should contain "Hi" from string syscall
        std::string consoleOutput = gui->getConsoleOutput();
        EXPECT_TRUE(consoleOutput.find("Hi") != std::string::npos) 
            << "Console output should contain 'Hi' but was: '" << consoleOutput << "'";
    }
}

// Test 4: Mixed output test - both integers and strings
TEST_F(EnhancedGuiConsoleTest, MixedOutputTest) {
    // Given: GUI simulator initialized
    ASSERT_TRUE(initialized) << "GUI should initialize successfully";
    
    // And: A program with both string and integer output
    std::string program = R"(
# Mixed output test
# Print integer 123
addi $v0, $zero, 1         # syscall 1: print_int
addi $a0, $zero, 123       # value 123
syscall

# Create and print string "OK"  
addi $t0, $zero, 0x4F      # 'O' = 0x4F
addi $t1, $zero, 0x4B      # 'K' = 0x4B
sll $t1, $t1, 8           # shift 'K' to position 1
add $t0, $t0, $t1         # combine O + K  
sw $t0, 0x1000($zero)     # store "OK\0\0"

addi $v0, $zero, 4        # syscall 4: print_string
addi $a0, $zero, 0x1000   # string address
syscall

# Print integer 456
addi $v0, $zero, 1        # syscall 1: print_int
addi $a0, $zero, 456      # value 456
syscall

addi $v0, $zero, 10       # syscall 10: exit
syscall
)";
    
    // When: We execute the program
    bool loaded = gui->loadProgram(program);
    EXPECT_TRUE(loaded) << "Program should load successfully";
    
    if (loaded) {
        gui->runProgram();
        
        // Then: Console should contain all expected outputs
        std::string consoleOutput = gui->getConsoleOutput();
        EXPECT_TRUE(consoleOutput.find("123") != std::string::npos) 
            << "Console should contain '123' but was: '" << consoleOutput << "'";
        EXPECT_TRUE(consoleOutput.find("OK") != std::string::npos) 
            << "Console should contain 'OK' but was: '" << consoleOutput << "'";
        EXPECT_TRUE(consoleOutput.find("456") != std::string::npos) 
            << "Console should contain '456' but was: '" << consoleOutput << "'";
    }
}

} // namespace mips
