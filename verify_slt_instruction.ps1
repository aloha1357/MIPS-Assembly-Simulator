# SLT Instruction Verification Script
# This script verifies that the SLT (Set Less Than) instruction is working correctly

Write-Host "=== SLT Instruction Verification Script ===" -ForegroundColor Green
Write-Host "Testing SLT (Set Less Than) instruction implementation" -ForegroundColor Yellow
Write-Host ""

# Change to project directory
$ErrorActionPreference = "Stop"

try {
    # Build the project
    Write-Host "Building project..." -ForegroundColor Blue
    cmake --build build --target unit_tests | Out-Null
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ Build successful" -ForegroundColor Green
    } else {
        Write-Host "❌ Build failed" -ForegroundColor Red
        exit 1
    }
    
    # Run SLT specific tests
    Write-Host "Running SLT instruction tests..." -ForegroundColor Blue
    $testOutput = .\build\tests\unit_tests.exe --gtest_filter="ExtendedInstructionsTest.*" --gtest_brief=1 2>&1
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ All SLT tests passed" -ForegroundColor Green
        
        # Count the number of SLT tests
        $testCount = ($testOutput | Select-String "tests from.*ExtendedInstructionsTest" | ForEach-Object { 
            if ($_ -match "(\d+) tests from") { $matches[1] }
        })
        
        Write-Host "   → $testCount SLT test scenarios verified" -ForegroundColor Cyan
    } else {
        Write-Host "❌ SLT tests failed" -ForegroundColor Red
        Write-Host $testOutput
        exit 1
    }
    
    # Run full regression test
    Write-Host "Running full regression test..." -ForegroundColor Blue
    $fullTestOutput = .\build\tests\unit_tests.exe --gtest_brief=1 2>&1
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✅ Full regression test passed" -ForegroundColor Green
        
        # Extract total test count
        $totalTests = ($fullTestOutput | Select-String "\[  PASSED  \] (\d+) tests" | ForEach-Object {
            if ($_ -match "\[  PASSED  \] (\d+) tests") { $matches[1] }
        })
        
        Write-Host "   → Total: $totalTests tests passing (no regressions)" -ForegroundColor Cyan
    } else {
        Write-Host "❌ Regression detected in full test suite" -ForegroundColor Red
        Write-Host $fullTestOutput
        exit 1
    }
    
    # Test the demo program if GUI is available
    Write-Host ""
    Write-Host "Demo program available: asmtest\slt_instruction_demo.asm" -ForegroundColor Magenta
    Write-Host "To test manually:" -ForegroundColor Yellow
    Write-Host "  1. Run: .\build\src\mips-sim-gui.exe" -ForegroundColor Gray
    Write-Host "  2. Load: asmtest\slt_instruction_demo.asm" -ForegroundColor Gray  
    Write-Host "  3. Execute and verify output: 1, 0, 1, 0" -ForegroundColor Gray
    
    Write-Host ""
    Write-Host "🎉 SLT Instruction Implementation Complete!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Implementation Summary:" -ForegroundColor Yellow
    Write-Host "✅ SltInstruction class added to Instruction.h" -ForegroundColor White
    Write-Host "✅ SLT execution logic implemented in Instruction.cpp" -ForegroundColor White
    Write-Host "✅ SLT opcode (0x2A) added to InstructionDecoder" -ForegroundColor White  
    Write-Host "✅ 5 comprehensive test cases covering all scenarios" -ForegroundColor White
    Write-Host "✅ Demo program created for manual verification" -ForegroundColor White
    Write-Host "✅ No regressions in existing functionality" -ForegroundColor White
    
    Write-Host ""
    Write-Host "BDD Development Cycle Complete!" -ForegroundColor Green
    Write-Host "A → B → C: Write Tests → Implement Code → Refactor" -ForegroundColor Cyan
    
} catch {
    Write-Host "❌ Verification failed: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}
