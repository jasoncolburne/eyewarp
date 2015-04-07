

Getting started in Ubuntu:

1) Get yasm, libx264-dev, libxvidcore-dev, libfdk-aac-dev
$ sudo apt-get install yasm libx264-dev libxvidcore-dev libfdk-aac-dev
1) Grab a copy of ffmpeg (N-69498-g7620d48) and configure:
$ ./configure --prefix=/usr/local --enable-version3 --enable-gpl --enable-shared --enable-nonfree --enable-libx264 --enable-libxvid --enable-libfdk-aac --enable-libvorbis --enable-libmp3lame --enable-libtheora --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libgsm
