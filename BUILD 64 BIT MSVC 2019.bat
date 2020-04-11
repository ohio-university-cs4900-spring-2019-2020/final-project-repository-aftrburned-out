REM Ensure cmake and cmake-gui are in your Environment's PATH
REM mkdir cwin64
REM cd cwin64
REM cmake -G "Visual Studio 16 2019" -A Win64 ../src/
REM start cmake-gui ../src/
cmake -G "Visual Studio 16 2019" -A x64 -S ./src/ -B ./cwin64
REM start cmake-gui ../src/
