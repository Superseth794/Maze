#!/bin/bash
PREMAKE_VERSION="premake5"

# check if the number of arguments is correct
if [[ $# < 1 ]]
then
    echo "----- Error: at least one argument is required -----"
    exit 1
fi

# remove any previous Makefile
rm Makefile

# run premake
echo ""
for param in "$*"
    do echo "----- Run premake with arguments $param -----"
done
echo ""

if [[ -x "$(command -v $PREMAKE_VERSION)" ]] # check if premake is in system default search path
then
$PREMAKE_VERSION --file=premake5.lua $@
elif [[ -x "$(command -v ./$PREMAKE_VERSION)" ]] # check if premake is in current directory
then
./$PREMAKE_VERSION --file=premake5.lua $@
else
echo "$PREMAKE_VERSION is required to lauch the program"
exit 1
fi

if [[ $? != 0 || ! -e "Makefile" ]]
then
    echo "----- Premake failed -----"
    exit 1
fi

# remove any previous executable
rm bin/Debug/Maze

# run makefile
echo ""
echo "----- Premake loaded -----"
echo ""

make all

echo ""

if [[ $? != 0 || ! -e "bin/Debug/Maze" ]]
then
    echo "----- Make failed -----"
    exit 1
fi

echo "----- Make achieved -----"
echo ""

# run program
echo "----- Starting program -----"
echo ""

export LD_LIBRARY_PATH=../../../SFML/lib
cd bin/Debug/
./Maze
unset LD_LIBRARY_PATH
cd ../../

echo ""
echo "----- Program ended -----"
