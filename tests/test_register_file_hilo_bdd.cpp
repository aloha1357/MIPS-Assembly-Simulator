/**
 * @file test_register_file_hilo_bdd.cpp
 * @brief BDD tests for RegisterFile HI/LO register support
 */

#include "RegisterFile.h"
#include <gtest/gtest.h>
#include <memory>

using namespace mips;

/**
 * @brief BDD test fixture for RegisterFile HI/LO functionality
 */
class RegisterFileHILOBDD : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        registerFile = std::make_unique<RegisterFile>();
    }

    void TearDown() override
    {
        registerFile.reset();
    }

    std::unique_ptr<RegisterFile> registerFile;
};

/**
 * @brief BDD Scenario: HI register basic read/write operations
 *
 * Given a RegisterFile with HI register support
 * When I write a value to HI register
 * Then I should be able to read the same value from HI register
 */
TEST_F(RegisterFileHILOBDD, HI_Register_Basic_ReadWrite)
{
    // Given: A fresh RegisterFile (initialized in SetUp)

    // When: Write a value to HI register
    uint32_t testValue = 0x12345678;
    registerFile->writeHI(testValue);

    // Then: Read the same value from HI register
    uint32_t readValue = registerFile->readHI();
    EXPECT_EQ(readValue, testValue);
}

/**
 * @brief BDD Scenario: LO register basic read/write operations
 *
 * Given a RegisterFile with LO register support
 * When I write a value to LO register
 * Then I should be able to read the same value from LO register
 */
TEST_F(RegisterFileHILOBDD, LO_Register_Basic_ReadWrite)
{
    // Given: A fresh RegisterFile (initialized in SetUp)

    // When: Write a value to LO register
    uint32_t testValue = 0x87654321;
    registerFile->writeLO(testValue);

    // Then: Read the same value from LO register
    uint32_t readValue = registerFile->readLO();
    EXPECT_EQ(readValue, testValue);
}

/**
 * @brief BDD Scenario: HI and LO registers are independent
 *
 * Given a RegisterFile with HI/LO register support
 * When I write different values to HI and LO registers
 * Then each register should maintain its own value independently
 */
TEST_F(RegisterFileHILOBDD, HI_LO_Registers_Independent)
{
    // Given: A fresh RegisterFile (initialized in SetUp)

    // When: Write different values to HI and LO registers
    uint32_t hiValue = 0xAAAAAAAA;
    uint32_t loValue = 0x55555555;
    registerFile->writeHI(hiValue);
    registerFile->writeLO(loValue);

    // Then: Each register should maintain its own value
    EXPECT_EQ(registerFile->readHI(), hiValue);
    EXPECT_EQ(registerFile->readLO(), loValue);
}

/**
 * @brief BDD Scenario: HI/LO registers reset to zero
 *
 * Given a RegisterFile with HI/LO register support
 * When I write values to HI/LO registers and then reset
 * Then both HI and LO registers should be zero after reset
 */
TEST_F(RegisterFileHILOBDD, HI_LO_Registers_Reset_To_Zero)
{
    // Given: A fresh RegisterFile with some values written
    registerFile->writeHI(0x12345678);
    registerFile->writeLO(0x87654321);

    // When: Reset the register file
    registerFile->reset();

    // Then: Both HI and LO should be zero
    EXPECT_EQ(registerFile->readHI(), 0);
    EXPECT_EQ(registerFile->readLO(), 0);
}
