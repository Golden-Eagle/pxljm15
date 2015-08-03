PXLJM2015!!!
=======


Building
========

Assuming cloned to directory named "pxljm15".

Linux
-----

```bash
cd pxljm15
mkdir build
cd build
cmake ..
make
cd ..
./build/bin/gecom

```

- You must run from the project root so resources can be loaded.

Windows (Visual Studio)
----------------------------

- Install cmake for Windows (win32 installer from http://www.cmake.org/cmake/resources/software.html)
- Add cmake to your path if it doesn't
- From cmd.exe, taking XX from the following table:

Product name       | XX
 ----------------- | ---
Visual Studio 2010 | 10
Visual Studio 2012 | 11
Visual Studio 2013 | 12
Visual Studio 2015 | 14

```bat
cd pxljm15
mkdir build
cd build
cmake .. -G "Visual Studio XX"
```

- Or for x64, substitute:

```bat
cmake .. -G "Visual Studio XX Win64"
```

- You now have a VS2013 solution
- In VS set 'gecom' as the startup project
  - Solution Explorer > 'gecom' > right click > Set as StartUp Project
- Set the working directory for 'gecom' to ```$(SolutionDir)..``` for all configurations
  - Solution Explorer > 'gecom' > right click > Properties > Configuration Properties > Debugging
  - Select 'All Configurations' from the configuration drop-down
  - This uses the project root as working directory so resources can be loaded
- You may need to manually rerun cmake if the source file listing changes

Adding Source Files
===================

This mainly applies to Visual Studio users. Create new source files manually in a subdirectory of ```/src```, probably ```/src/gecom```. Then re-run cmake to add them to the project (for VS, this should not affect the working directory).

Controls
========

Window Key Bindings
-------------------
- \` toggles mouselook
- WASD control horizontal movement

Console Commands
----------------
-There are no console Commands
