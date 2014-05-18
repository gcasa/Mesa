-=-=-=-=-
TO BUILD ON WINDOWS:

Extract this archive to a directory of your choice,
example:
 c:\Mesa.

Open (in MS Visual C++ 6.0) the project file
C:\Mesa\Source\Dawn\Dawn.dsw

Select a Debug or Release build.

Rebuild All.

Note: This does not build the pre-built Windows XNS packet driver.
You need a DDK to do that.  Further, I think my XNS packet driver
stopped working in XP service pack 1, so I may need to update it.

The executable will be built as:
c:\Mesa\Targets\Debug\Bin\Dawn.exe
or
c:\Mesa\Targets\Release\Bin\Dawn.exe

To run it, place a Dawn virtual disk file called Dawn.dsk
as the same directory as the executable.

-=-=-=-=-
TO BUILD ON LINUX:

Extract this archive to a directory of your choice,
example:
 /root/Mesa

cd /root/Mesa

Edit the file Makefile.linux and ensure the MESA_ROOT_DIR
variable refers to this directory.

example:
 MESA_ROOT_DIR := /root/mesa

make -f Makefile.linux


This will create libPrincOps.a and libPrincOps.so
in the same directory.

libPrincOps.a is a static library and libPrincOps.so
is a shared library.  These libraries only contain the
core PrincOps Mesa Processor code.  I have not created
a Linux equivalent of the Dawn host application yet.
Also, I've never actually run these libraries, but
they do build.

Enjoy,
-Don Woodward