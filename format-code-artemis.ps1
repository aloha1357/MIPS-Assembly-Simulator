#!/usr/bin/env pwsh
# PowerShell script to format code using clang-format

Write-Host "Formatting C++ code with clang-format..." -ForegroundColor Green

# Check if clang-format is available
if (-not (Get-Command clang-format -ErrorAction SilentlyContinue)) {
    Write-Host "Error: clang-format not found in PATH" -ForegroundColor Red
    Write-Host "Please install clang-format or add it to your PATH" -ForegroundColor Red
    exit 1
}

# Get all C++ source files
$sourceFiles = Get-ChildItem -Recurse -Include "*.cpp", "*.h", "*.hpp" -Path "src", "." | 
               Where-Object { $_.Directory.Name -ne "build" -and $_.Directory.Name -notlike "*build*" } |
               ForEach-Object { $_.FullName }

if ($sourceFiles.Count -eq 0) {
    Write-Host "No C++ source files found" -ForegroundColor Yellow
    exit 0
}

Write-Host "Found $($sourceFiles.Count) source files to format:" -ForegroundColor Cyan
foreach ($file in $sourceFiles) {
    $relativePath = Resolve-Path -Path $file -Relative
    Write-Host "  $relativePath" -ForegroundColor Gray
}

# Format each file
foreach ($file in $sourceFiles) {
    $relativePath = Resolve-Path -Path $file -Relative
    Write-Host "Formatting: $relativePath" -ForegroundColor Yellow
    
    try {
        clang-format -i $file
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Warning: clang-format returned non-zero exit code for $relativePath" -ForegroundColor Yellow
        }
    }
    catch {
        Write-Host "Error formatting $relativePath : $_" -ForegroundColor Red
    }
}

Write-Host "Code formatting completed!" -ForegroundColor Green
Write-Host "You can now commit your formatted code to meet Artemis requirements." -ForegroundColor Cyan
