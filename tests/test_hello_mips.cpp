#include "../src/Cpu.h"
#include "../src/Instruction.h"
#include "../src/Memory.h"
#include "../src/RegisterFile.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace mips;

class HelloMipsTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        cpu = std::make_unique<Cpu>();

        // 重新導向輸出用於測試
        original_cout_buf = std::cout.rdbuf();
        std::cout.rdbuf(output_stream.rdbuf());
    }

    void TearDown() override
    {
        // 恢復原本的輸出
        std::cout.rdbuf(original_cout_buf);
    }

    std::unique_ptr<Cpu> cpu;
    std::ostringstream   output_stream;
    std::streambuf*      original_cout_buf;
};

TEST_F(HelloMipsTest, TestHelloMipsSimple)
{
    std::string assembly_code = R"(
        # Hello MIPS - 簡化版本測試
        addi $t0, $zero, 0x48       # 'H' = 0x48
        addi $t1, $zero, 0x69       # 'i' = 0x69
        sll $t1, $t1, 8            # 將 'i' 移到位元組位置 1
        add $t0, $t0, $t1          # 組合 H + i = "Hi\0\0"
        sw $t0, 0x1000($zero)      # 存儲到記憶體位址 0x1000
        
        # 輸出字串 "Hi"
        addi $v0, $zero, 4         # syscall 4: print_string
        addi $a0, $zero, 0x1000    # 字串起始位址
        syscall                    # 輸出: "Hi"
        
        # 輸出一個數字
        addi $v0, $zero, 1         # syscall 1: print_int  
        addi $a0, $zero, 123       # 測試數字 123
        syscall
        
        # 正常結束程式
        addi $v0, $zero, 10        # syscall 10: exit
        syscall
    )";

    // 載入程式碼
    cpu->loadProgramFromString(assembly_code);

    // 執行程式直到結束
    bool      exit_called = false;
    int       cycle_count = 0;
    const int max_cycles  = 100;

    while (!exit_called && cycle_count < max_cycles)
    {
        cpu->tick();

        // 檢查是否呼叫了 exit syscall
        if (cpu->getRegisterFile().read(2) == 10)
        { // $v0 == 10 (exit)
            exit_called = true;
        }

        cycle_count++;
    }

    EXPECT_TRUE(exit_called) << "程式應該正常結束（呼叫 exit syscall）";
    EXPECT_LT(cycle_count, max_cycles) << "程式執行不應該超過最大循環數";

    // 檢查記憶體內容是否正確存儲了 "Hi"
    uint32_t stored_value = cpu->getMemory().readWord(0x1000);
    EXPECT_EQ(stored_value & 0xFF, 0x48) << "第一個字節應該是 'H' (0x48)";
    EXPECT_EQ((stored_value >> 8) & 0xFF, 0x69) << "第二個字節應該是 'i' (0x69)";
    EXPECT_EQ((stored_value >> 16) & 0xFF, 0x00) << "第三個字節應該是 null terminator";

    // 檢查輸出內容
    std::string output = cpu->getConsoleOutput();
    EXPECT_TRUE(output.find("Hi") != std::string::npos) << "輸出應該包含 'Hi'";
    EXPECT_TRUE(output.find("123") != std::string::npos) << "輸出應該包含 '123'";

    std::cout << "Hello MIPS 簡化版測試完成！輸出: " << output << std::endl;
}

TEST_F(HelloMipsTest, TestSllInstruction)
{
    std::string assembly_code = R"(
        # 測試 SLL 指令是否正確實作
        addi $t0, $zero, 0x69       # 'i' = 0x69
        sll $t1, $t0, 8            # 將 $t0 左移 8 位，結果存到 $t1
        addi $v0, $zero, 10        # exit syscall
        syscall
    )";

    cpu->loadProgramFromString(assembly_code);

    // 執行足夠的循環來完成 SLL 指令
    for (int i = 0; i < 20; i++)
    {
        cpu->tick();
        if (cpu->getRegisterFile().read(2) == 10)
            break;
    }

    // 檢查 SLL 指令的結果
    uint32_t result = cpu->getRegisterFile().read(9); // $t1
    EXPECT_EQ(result, 0x6900) << "SLL 指令應該將 0x69 左移 8 位得到 0x6900";

    std::cout << "SLL 指令測試完成！" << std::endl;
}
