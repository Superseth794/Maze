/Users/jaraxus/Downloads/premake5 --file=premake5.lua gmake2
echo "\n----- Premake loaded -----\n"
make
echo "\n----- Make achieved -----\n"
echo "----- Starting program -----\n"
export LD_LIBRARY_PATH=../../../SFML/lib
cd bin/Debug/
exec ./Maze
unset LD_LIBRARY_PATH
cd ../../
echo "\n----- Program ended -----\n"
