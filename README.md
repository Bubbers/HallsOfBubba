# HallsOfBubba

# Build with MXE

cd ~/mxe
make MXE_TARGETS='x86_64-w64-mingw32.static' sfml vorbis glew freeglut freetype assimp

cd ~/HallsOfBubba
mkdir build
cd build
i686-w64-mingw32.static-cmake -DSFML_STATIC_LIBRARIES=TRUE ..

# Run .exe with wine
sudo dpkg --add-architecture i386 
wget https://dl.winehq.org/wine-builds/Release.key
sudo apt-key add Release.key
sudo apt-add-repository https://dl.winehq.org/wine-builds/ubuntu/
sudo apt-get update
sudo apt-get install --install-recommends winehq-staging
