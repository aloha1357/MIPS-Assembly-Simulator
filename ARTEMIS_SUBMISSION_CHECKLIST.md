# Artemis Submission Checklist

## Pre-Submission Verification

### 1. Code Formatting ✓
- [ ] Added `.clang-format` configuration file
- [ ] Run `./format-code.ps1` to format all source files
- [ ] Verify code follows consistent formatting style

### 2. Build System ✓
- [ ] CMakeLists.txt properly configured for Qt6, Catch2, and GoogleTest
- [ ] Clean build works: `cmake -B build && cmake --build build`
- [ ] All targets compile without errors
- [ ] Both Debug and Release configurations work

### 3. Testing ✓
- [ ] All tests pass: `ctest --test-dir build`
- [ ] Catch2 tests execute successfully
- [ ] GoogleTest tests execute successfully
- [ ] No test failures or crashes

### 4. Report ✓
- [ ] `assignment/report.md` is complete and comprehensive
- [ ] Report follows Markdown formatting guidelines
- [ ] All sections properly documented:
  - [ ] Project Overview
  - [ ] Architecture Design
  - [ ] Implementation Details
  - [ ] Testing Strategy
  - [ ] Build Instructions
  - [ ] Conclusion

### 5. Code Quality
- [ ] All source files properly formatted with clang-format
- [ ] No compilation warnings in Release mode
- [ ] Proper header guards and includes
- [ ] Clear code documentation and comments

### 6. Project Structure
```
project/
├── CMakeLists.txt          ✓ Main build configuration
├── .clang-format          ✓ Code formatting rules
├── assignment/
│   └── report.md          ✓ Project report
├── src/                   ✓ Source code
├── tests/                 ✓ Test files
├── lib.cpp               ✓ Library implementation
├── lib.hpp               ✓ Library header
├── cli.cpp               ✓ CLI tool
├── gui.cpp               ✓ GUI tool
├── tests.cpp             ✓ Catch2 tests
└── tests2.cpp            ✓ GoogleTest tests
```

## Quick Verification Commands

```powershell
# 1. Format code
.\format-code.ps1

# 2. Clean build
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
cmake -B build
cmake --build build

# 3. Run tests
ctest --test-dir build --output-on-failure

# 4. Verify executables
.\build\my_cli_tool.exe
.\build\my_gui_tool.exe
.\build\my_catch2_tests.exe
.\build\my_gtest_tests.exe
```

## Artemis Environment Notes

- Build environment provides Qt6, Catch2, and GoogleTest
- Code should compile cleanly with no warnings
- All tests must pass
- Report should be in Markdown format
- Code formatting should be consistent (clang-format)

## Final Submission Steps

1. **Final Code Review**: Ensure all code is clean and well-documented
2. **Build Verification**: Complete clean build with no errors/warnings  
3. **Test Verification**: All tests pass consistently
4. **Report Review**: Report is complete and professional
5. **Format Check**: All code properly formatted with clang-format
6. **Submit to Artemis**: Upload your complete project

---

**Good luck with your submission! You have a well-developed MIPS simulator project.** 🚀
