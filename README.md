# Ascii Video Player In C ™️
no one made one that is actually good so i did it smh

made in c so it actually...  you know... runs at a usable speed

> [!WARNING]  
> very beta might not be actually good

## how
simple
1. steal libraries (don't worry they're open source)
```sh
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
wget https://raw.githubusercontent.com/nothings/stb/master/deprecated/stb_image_resize.h
wget https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h
```
2. compile libraries
```sh
cc -O3 -c libs.c -o libs.o
```
3. compile main program
```sh
cc -O3 asciivideo.c -o asciivideo -lm libs.o
```

## ok but how
good question

so um it doesnt really do this itself yet so you have to decompose the video in frames !!!

so yea make sure you have ffmpeg for this

1. make a new folder of desired name
2. go to it
```sh
cd your_folder
```
3. do it
```sh
ffmpeg -i /path/to/your/video -r 60 %d.png audio.mp3
```

you can now go back to the main directory and run the program
```sh
./asciivideo your_folder
```

## um um windows
[the windows subsystem for linux in question:](https://learn.microsoft.com/en-us/windows/wsl/install)

## shoutouts to
[stb](https://github.com/nothings/stb) and [miniaudio](https://github.com/mackron/miniaudio) for doing all the hard work lmao

[tepigers](https://github.com/Tepigers) for forcing me to make this