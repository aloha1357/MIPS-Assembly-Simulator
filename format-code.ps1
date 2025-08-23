#!/usr/bin/env powershell
# Script to format all source code using clang-format

Write-Host "Formatting source code with clang-format..."

# Find all source files
$sourceFiles = Get-ChildItem -Recurse -Include "*.cpp", "*.h", "*.hpp" -Path "src", "tests", "." | Where-Object { $_.Directory.Name -ne "build" -and $_.Directory.Name -ne "_deps" }

if ($sourceFiles.Count -eq 0) {
    Write-Host "No source files found to format."
    exit 1
}

Write-Host "Found $($sourceFiles.Count) source files to format:"
foreach ($file in $sourceFiles) {
    Write-Host "  $($file.FullName)"
}

# Check if clang-format is available
try {
    $formatVersion = clang-format --version
    Write-Host "Using: $formatVersion"
} catch {
    Write-Host "ERROR: clang-format not found. Please install LLVM/Clang tools."
    Write-Host "You can download from: https://releases.llvm.org/download.html"
    exit 1
}

# Format each file
foreach ($file in $sourceFiles) {
    Write-Host "Formatting: $($file.Name)"
    clang-format -i $file.FullName
}

Write-Host "Code formatting complete!"
Write-Host ""
Write-Host "Next steps for Artemis submission:"
Write-Host "1. Build the project: cmake -B build && cmake --build build"
Write-Host "2. Run tests: ctest --test-dir build"
Write-Host "3. Verify report.md is complete"
Write-Host "4. Submit to Artemis"
