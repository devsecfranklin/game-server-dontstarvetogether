toolchain_windows="../toolchain/windows.cmake"
toolchain_linux="../toolchain/linux.cmake"

FYP_OPTIONS="-DFYP_CCACHE=ON"
CMAKE_OPTIONS="-DCMAKE_EXPORT_COMPILE_COMMANDS=True\
	       -DCMAKE_BUILD_TYPE=RelWithDebInfo"
COMBINED_OPTIONS="$FYP_OPTIONS $CMAKE_OPTIONS -GNinja"

echo "[$0] Recreating build directories"
rm -rf build_linux
rm -rf build_windows
mkdir build_linux
mkdir build_windows

echo "[$0] Running cmake with these options:"
echo "$COMBINED_OPTIONS"
echo "[$0] -- End of options"

echo "[$0] For Linux build using toolchain $toolchain_linux"
cd build_linux;cmake .. $COMBINED_OPTIONS -DCMAKE_TOOLCHAIN_FILE=$toolchain_linux; ninja
cd -

#echo "[$0] For Windows build using toolchain $toolchain_windows"
#cd build_windows;cmake .. $COMBINED_OPTIONS  -DCMAKE_TOOLCHAIN_FILE=$toolchain_windows; ninja
