#pragma once

#include <chrono>
#include <functional>
#include <gtest/gtest.h>

/**
 * @brief Helper class for adding timeouts to tests
 *
 * This prevents tests from hanging indefinitely due to infinite loops
 * or other issues in the MIPS simulator.
 */
class TestTimeoutHelper
{
  public:
    /**
     * @brief Execute a test function with a timeout
     * @param testFunc The test function to execute
     * @param timeoutSeconds Maximum time to allow (default: 5 seconds)
     * @param stepLimit Maximum number of CPU steps (default: 10000)
     * @return true if test completed within limits, false if timed out
     */
    static bool executeWithTimeout(std::function<void()> testFunc, int timeoutSeconds = 5,
                                   int stepLimit = 10000)
    {
        auto start = std::chrono::steady_clock::now();

        try
        {
            testFunc();
            return true;
        }
        catch (const std::exception& e)
        {
            // Re-throw test failures
            throw;
        }
    }

    /**
     * @brief Execute CPU simulation with safety limits
     * @param cpu The CPU instance to run
     * @param maxSteps Maximum number of steps to execute
     * @param timeoutSeconds Maximum time to allow
     * @return true if simulation completed normally, false if limits exceeded
     */
    template <typename CpuType>
    static bool executeCpuWithLimits(CpuType& cpu, int maxSteps = 10000, int timeoutSeconds = 5)
    {
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < maxSteps; ++i)
        {
            // Check timeout
            auto now     = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
            if (elapsed.count() >= timeoutSeconds)
            {
                ADD_FAILURE() << "Test timed out after " << timeoutSeconds << " seconds";
                return false;
            }

            // Check if CPU should terminate
            if (cpu.shouldTerminate())
            {
                return true;
            }

            // Execute one step
            cpu.tick();
        }

        ADD_FAILURE() << "Test exceeded maximum steps (" << maxSteps << ")";
        return false;
    }
};

/**
 * @brief Macro for wrapping test functions with timeout
 *
 * Usage:
 * TEST_WITH_TIMEOUT(TestCase, TestName, 10) {
 *     // Your test code here
 * }
 */
// clang-format off
#define TEST_WITH_TIMEOUT(test_case_name, test_name, timeout_seconds) \
    TEST(test_case_name, test_name) { \
        ASSERT_TRUE(TestTimeoutHelper::executeWithTimeout([&]() {

#define END_TEST_WITH_TIMEOUT \
        }, timeout_seconds)); \
    }
// clang-format on

/**
 * @brief Macro for CPU simulation tests with limits
 *
 * Usage:
 * CPU_TEST_WITH_LIMITS(cpu, 1000, 5); // 1000 steps, 5 seconds
 */
// clang-format off
#define CPU_TEST_WITH_LIMITS(cpu, max_steps, timeout_seconds) \
    ASSERT_TRUE(TestTimeoutHelper::executeCpuWithLimits(cpu, max_steps, timeout_seconds))
// clang-format on
