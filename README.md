# Welcome to Jam Jam Revolution!

![alt text](https://img.itch.zone/aW1hZ2UvOTAxOTAvNDIzNDAwLnBuZw==/original/OpPn5t.png)

This game was made during the GBJAM 5 event. GB standing for Game Boy.
The goal was to make a game using only 4 colors, have a resolution of
160 by 144 pixels, and have the final product be gameboy themed. 

Jam Jam Revolution, or just JJR for short, is a simple rhythm game about
hitting buttons and arrows while listening to music. If you're familiar
with games like Dance Dance Revolution or Project Diva then you'll know
precisely what I'm going on about! If not, don't fret! Give the game a go
yourself. It's up for download on itch.io [here](https://kenny-designs.itch.io/jam-jam-revolution).
	
This game was made in about 10 days for the jam. That said, there's
plenty of improvements that can be made! I know there are resources
being loaded when they don't need to be and I ran out of time before 
I could implement an algorithm to have the prompts match the music.
Controls could be improved and more levels and effects could be added.
This is a great project for anyone to come and improve and I totally
welcome it! 

One last thing, the game itself was made using the Win32 API, DirectX9,
and C++. The IDE I utilized was Visual Studio 2015. Photoshop was used
to make the pixel art and I included the Photoshop files. Feel free to 
alter and use them as a base. Each image was saved as a .png or .bmp at
five times its original size for the actual game.

If you can't get the project to compile try a few things. One, make sure 
the project's character set is multi-byte. Also, be sure you have the
DirectX June 2010 SDK installed. You can get it [here](https://www.microsoft.com/en-us/download/details.aspx?id=6812).
	
You should then have a file path that looks like this:

```
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)
```

You will need the files in both '/Include' and '/Lib/x86' in your project
from the directory above. This is so you can use DirectX files such as d3dx9.h.
