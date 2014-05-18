call c:\NTDDK\bin\setenv.bat c:\NTDDK free
cd %1
build -cZ
erase %1\..\..\Targets\Release\Bin\i386\xnsdrw.inf
copy %1\xnsdrw.inf %1\..\..\Targets\Release\Bin\i386\xnsdrw.inf