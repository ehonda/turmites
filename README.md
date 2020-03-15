# turmites
This is a simulator for [turmites](https://en.wikipedia.org/wiki/Turmite), which are a generalization of [Langton's Ant](https://en.wikipedia.org/wiki/Langton%27s_ant). It is very much unfinished now, but I hope to be getting back to it sometime soon (Although I am also considering implementing it in Haskell right now).

The project uses a number of external libraries. [SDL2](https://www.libsdl.org/download-2.0.php) is used to process input and draw the cells. [Cereal](https://uscilab.github.io/cereal/) is used to serialize turmites. [Boost](https://www.boost.org/) right now is mainly used for implementing observers via [Signals](https://www.boost.org/doc/libs/1_72_0/doc/html/signals2.html). Finally, [Google Test](https://github.com/google/googletest) is used for writing the tests in the accompanying test project. To build the project, these libraries have to be located in certain directories pointed to by certain environment variables (See the [Visual Studio Project Files](https://github.com/ehonda/turmites/blob/master/turmites/app/app.vcxproj)). If you would like a more detailed description on how to build everything, please open an issue and i will take the time to describe it, but really the whole dependency management should be organized better and not done manually like this (One idea is to use the [Conan](https://conan.io/) package management system).

The simulation can be controlled by using the following keys:

"l" - Load a turmite savefile

"s" - save the current turmite

"r" - start new random turmite with the same gridsize and number of internal/cell states as before

"n" - start new random turmite, specifying gridsize and number of internal/cell states

"f" - show fps

"+" - make simulation go faster

"-" - make simulation go slower
