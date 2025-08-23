#!/usr/bin/env pwsh
# PowerShell script to verify Artemis readiness

Write-Host "=== ARTEMIS READINESS CHECKER ===" -ForegroundColor Cyan
Write-Host ""

$errors = 0
$warnings = 0

# Function to check file existence
function Test-FileExists {
    param($path, $description, $required = $true)
    
    if (Test-Path $path) {
        Write-Host "✓ $description found: $path" -ForegroundColor Green
        return $true
    } else {
        if ($required) {
            Write-Host "✗ $description missing: $path" -ForegroundColor Red
            $script:errors++
        } else {
            Write-Host "⚠ $description not found: $path" -ForegroundColor Yellow
            $script:warnings++
        }
        return $false
    }
}

# Function to check CMake configuration
function Test-CMakeConfig {
    Write-Host "Checking CMake configuration..." -ForegroundColor Yellow
    
    if (-not (Test-FileExists "CMakeLists.txt" "Root CMakeLists.txt")) {
        return
    }
    
    $content = Get-Content "CMakeLists.txt" -Raw
    
    # Check for required libraries
    $requiredLibs = @("Catch2", "GTest", "Qt6")
    foreach ($lib in $requiredLibs) {
        if ($content -match "find_package\s*\(\s*$lib") {
            Write-Host "✓ $lib library configured" -ForegroundColor Green
        } else {
            Write-Host "✗ $lib library not found in CMakeLists.txt" -ForegroundColor Red
            $script:errors++
        }
    }
    
    # Check for test targets
    if ($content -match "my_catch2_tests|my_gtest_tests") {
        Write-Host "✓ Test targets configured" -ForegroundColor Green
    } else {
        Write-Host "✗ Test targets not found" -ForegroundColor Red
        $script:errors++
    }
}

# Check essential files
Write-Host "Checking essential files..." -ForegroundColor Yellow
Test-FileExists "CMakeLists.txt" "Root CMakeLists.txt"
Test-FileExists "src/CMakeLists.txt" "Source CMakeLists.txt"
Test-FileExists "assignment/report.md" "Project report"
Test-FileExists ".clang-format" "Clang-format configuration"

# Check source files
Write-Host "`nChecking source files..." -ForegroundColor Yellow
Test-FileExists "src/MipsSimulatorAPI.h" "API header"
Test-FileExists "src/MipsSimulatorAPI.cpp" "API implementation"
Test-FileExists "src/Cpu.h" "CPU header"
Test-FileExists "src/Memory.h" "Memory header"
Test-FileExists "src/RegisterFile.h" "RegisterFile header"
Test-FileExists "src/Assembler.h" "Assembler header"

# Check test files
Write-Host "`nChecking test files..." -ForegroundColor Yellow
Test-FileExists "tests.cpp" "Catch2 tests"
Test-FileExists "tests2.cpp" "GoogleTest tests"

# Check main programs
Write-Host "`nChecking main programs..." -ForegroundColor Yellow
Test-FileExists "src/artemis_main.cpp" "Artemis-compatible main program"
Test-FileExists "src/main.cpp" "CLI main program" $false

# Check CMake configuration
Write-Host "`n" 
Test-CMakeConfig

# Check for build directories (should be clean for Artemis)
Write-Host "`nChecking build cleanliness..." -ForegroundColor Yellow
$buildDirs = @("build", "build-headless", "debug")
foreach ($dir in $buildDirs) {
    if (Test-Path $dir) {
        Write-Host "⚠ Build directory exists: $dir (consider cleaning before submission)" -ForegroundColor Yellow
        $warnings++
    }
}

# Test basic compilation
Write-Host "`nTesting CMake configuration..." -ForegroundColor Yellow
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    try {
        $tempBuild = "temp_build_test"
        if (Test-Path $tempBuild) {
            Remove-Item $tempBuild -Recurse -Force
        }
        
        Write-Host "Running CMake configuration test..." -ForegroundColor Gray
        $output = cmake -B $tempBuild -S . 2>&1
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "✓ CMake configuration successful" -ForegroundColor Green
        } else {
            Write-Host "✗ CMake configuration failed" -ForegroundColor Red
            Write-Host "CMake output:" -ForegroundColor Gray
            Write-Host $output -ForegroundColor Gray
            $errors++
        }
        
        # Clean up
        if (Test-Path $tempBuild) {
            Remove-Item $tempBuild -Recurse -Force
        }
    }
    catch {
        Write-Host "✗ CMake test failed: $_" -ForegroundColor Red
        $errors++
    }
} else {
    Write-Host "⚠ CMake not found, skipping configuration test" -ForegroundColor Yellow
    $warnings++
}

# Check code formatting
Write-Host "`nChecking code formatting..." -ForegroundColor Yellow
if (Get-Command clang-format -ErrorAction SilentlyContinue) {
    Write-Host "✓ clang-format available" -ForegroundColor Green
    Write-Host "  Run './format-code-artemis.ps1' to format code before submission" -ForegroundColor Cyan
} else {
    Write-Host "⚠ clang-format not found (install for proper formatting)" -ForegroundColor Yellow
    $warnings++
}

# Summary
Write-Host "`n=== SUMMARY ===" -ForegroundColor Cyan
if ($errors -eq 0) {
    Write-Host "✓ Project appears ready for Artemis submission!" -ForegroundColor Green
    if ($warnings -gt 0) {
        Write-Host "⚠ $warnings warning(s) found (see above)" -ForegroundColor Yellow
    }
} else {
    Write-Host "✗ $errors error(s) found - please fix before submitting" -ForegroundColor Red
    if ($warnings -gt 0) {
        Write-Host "⚠ $warnings warning(s) also found" -ForegroundColor Yellow
    }
}

Write-Host "`nRecommended submission checklist:" -ForegroundColor Cyan
Write-Host "1. Run './format-code-artemis.ps1' to format all code" -ForegroundColor White
Write-Host "2. Test build: cmake -B build && cmake --build build" -ForegroundColor White
Write-Host "3. Run tests: ./build/my_catch2_tests && ./build/my_gtest_tests" -ForegroundColor White
Write-Host "4. Verify report.md is complete and well-formatted" -ForegroundColor White
Write-Host "5. Clean build directories: Remove-Item build, build-headless -Recurse -Force" -ForegroundColor White
Write-Host "6. Submit to Artemis" -ForegroundColor White

exit $errors
