echo off
cd bin\Release\
PATH=..\..\externals\OgreSDK_vc10_v1-8-1\bin\release;
REM You can specify the .obj file you want to load with : -file "yourfile.obj"
BezierLoaderTest.exe -file ..\..\doc\bezier.obj