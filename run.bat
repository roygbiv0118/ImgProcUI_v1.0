@ECHO OFF

::SET QT_DIR=D:\Qt\Qt5.6.0\5.6\msvc2013
SET QT_DIR=D:\SDK\qt-5.x_x86_vc120\5.5\msvc2013
SET OPENCV_DIR=D:\SDK\opencv_3.0.0\build

rd /s/q build
md build
cd build

cmake.exe -L -DCMAKE_CXX_FLAGS_RELWITHDEBINFO:STRING="/MD /Zi /Od /Ob0 /D NDEBUG" -DCMAKE_C_FLAGS_RELWITHDEBINFO:STRING="/MD /Zi /Od /Ob0 /D NDEBUG" -DX86:BOOL=ON -DX64:BOOL=OFF -DQT_DIR:PATH=%QT_DIR% -DOPENCV_DIR:PATH=%OPENCV_DIR% ../ -G "Visual Studio 12 2013"

PAUSE
