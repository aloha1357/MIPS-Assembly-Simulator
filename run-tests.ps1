#!/usr/bin/env pwsh
# MIPS Simulator Test Runner Script
# Usage: .\run-tests.ps1 [test_group]

param(
    [string]$TestGroup = "all"
)

$BuildDir = "build-new"

# Check if build directory exists
if (-not (Test-Path $BuildDir)) {
    Write-Host "Build directory $BuildDir does not exist, building..." -ForegroundColor Yellow
    cmake -S . -B $BuildDir
    cmake --build $BuildDir
}

Write-Host "MIPS Simulator Test Runner" -ForegroundColor Green
Write-Host "==========================" -ForegroundColor Green

switch ($TestGroup.ToLower()) {
    "all" {
        Write-Host "Running all tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe"
    }
    "smoke" {
        Write-Host "Running smoke tests (quick verification)..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="CpuTest.InitialState:MemoryTest.WriteAndRead:RegisterFileTest.WriteAndRead:InstructionTest.AddInstructionBasic"
    }
    "core" {
        Write-Host "Running core functionality tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="CpuTest.*:MemoryTest.*:RegisterFileTest.*:PipelineTest.*:InstructionDecoderTest.*:InstructionTest.*"
    }
    "logical" {
        Write-Host "Running logical instruction tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*Logical*:*logical*:And*:Or*:Xor*:Nor*:*Andi*:*Ori*:*Xori*"
    }
    "shift" {
        Write-Host "Running shift instruction tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*Sll*:*Srl*:*Sra*:*SLL*:*SRL*:*SRA*"
    }
    "branch" {
        Write-Host "Running branch/jump instruction tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*BEQ*:*BNE*:*BLEZ*:*BGTZ*:*JR*:*JAL*:*JALR*"
    }
    "memory" {
        Write-Host "Running memory instruction tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*LW*:*SW*:*LB*:*SB*:*LH*:*SH*:*LBU*:*LHU*"
    }
    "arithmetic" {
        Write-Host "Running arithmetic instruction tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*ADD*:*SUB*:*MULT*:*DIV*:*SLT*:*Addi*:*Add*:*Sub*"
    }
    "bdd" {
        Write-Host "Running BDD tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*BDD*:*Bdd*"
    }
    "integration" {
        Write-Host "Running integration tests..." -ForegroundColor Cyan
        & "$BuildDir\tests\mips_tests.exe" --gtest_filter="*Integration*:*EndToEnd*:*AssemblerDebugTest*:*SyscallTest*"
    }
    "list" {
        Write-Host "Available test groups:" -ForegroundColor Yellow
        Write-Host "  all         - Run all tests" -ForegroundColor White
        Write-Host "  smoke       - Quick smoke tests (basic functionality)" -ForegroundColor White
        Write-Host "  core        - Core functionality tests (CPU, Memory, Pipeline)" -ForegroundColor White
        Write-Host "  logical     - Logical instruction tests (AND, OR, XOR, NOR)" -ForegroundColor White
        Write-Host "  shift       - Shift instruction tests (SLL, SRL, SRA)" -ForegroundColor White
        Write-Host "  branch      - Branch/jump instruction tests (BEQ, JR, JAL)" -ForegroundColor White
        Write-Host "  memory      - Memory instruction tests (LW, SW, LB, SB)" -ForegroundColor White
        Write-Host "  arithmetic  - Arithmetic instruction tests (ADD, SUB, MULT, DIV)" -ForegroundColor White
        Write-Host "  bdd         - BDD behavior-driven tests" -ForegroundColor White
        Write-Host "  integration - Integration tests" -ForegroundColor White
        Write-Host ""
        Write-Host "Usage: .\run-tests.ps1 [test_group]" -ForegroundColor Green
        return
    }
    default {
        Write-Host "Unknown test group: $TestGroup" -ForegroundColor Red
        Write-Host "Use '.\run-tests.ps1 list' to see available test groups" -ForegroundColor Yellow
        exit 1
    }
}

Write-Host ""
Write-Host "Test execution completed!" -ForegroundColor Green
