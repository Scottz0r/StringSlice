cmake.exe -B Build

Push-Location Build

cmake.exe --build .

.\Debug\StringSliceTests.exe

Pop-Location
