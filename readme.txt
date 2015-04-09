

Getting started in Ubuntu:

1) Get yasm, libx264-dev, libxvidcore-dev, libfdk-aac-dev
$ sudo apt-get install yasm libx264-dev libxvidcore-dev libfdk-aac-dev libmp3lame-dev libtheora-dev libvorbis-dev libopencore-amrnb-dev libopencore-amrwb-dev libgsm1-dev zlib1g-dev

2) Grab a copy of ffmpeg (N-69498-g7620d48) and configure, build and install:
ffmpeg $ ./configure --prefix=/usr/local --enable-version3 --enable-gpl --enable-shared --enable-nonfree --enable-avresample --enable-libx264 --enable-libxvid --enable-libfdk-aac --enable-libvorbis --enable-libmp3lame --enable-libtheora --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libgsm
ffmpeg$ make
ffmpeg$ sudo make install

3) Install Ruby, or follow instructions at https://rvm.io/rvm/install
for a robust solution
$ sudo apt-get install ruby

4) Export target and build environment variables.
$ export RED_TARGET=linux64_x86
$ export RED_BUILD=release

5) Fetch the RedRum build system and built as per instructions in readme.txt.
src$ git clone git@github.com:redbeardenterprises/redrum.git
src$ cd redrum
redrum$ make dist

6) Copy redrum package to working directory and unzip.
eyewarp$ cp ../redrum/release/redrum-0.1.7-release.zip .
eyewarp$ unzip redrum-0.1.7-release.zip

7) Fetch the Red C-Toolkit and build as per instructions in readme.txt.
src$ git clone git@github.com:redbeardenterprises/red.git
src$ cd red
red$ make test
red$ make dist

8) Copy red package to pkg destination and unzip.
eyewarp$ mkdir pkg
eyewarp$ cd pkg
pkg$ cp ../../red/release/red-0.1.2-linux64_x86-release.zip .
pkg$ unzip red-0.1.2-linux64_x86-release.zip

8) Build eyewarp.
eyewarp$ make

9) Play.
eyewarp$
