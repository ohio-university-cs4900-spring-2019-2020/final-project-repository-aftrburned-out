REM Ensure cmake and cmake-gui are in your Environment's PATH
mkdir cwin64
cd cwin64
cmake -G "Visual Studio 15 2017 Win64" ../src/
start cmake-gui ../src/
