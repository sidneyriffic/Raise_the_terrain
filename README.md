Terrain generator  

Compile using *gcc -lm -lSDL2 -o terrain*  
Requires SDL2 library  
  
**Usage:** terrain heightmap  
  
Heightmap file is a set of integers separated by spaces and newlines. Every newline starts a new row, and spaces in a line generates a new column in that row.  
  
Once open you can use the following keys:  
1. left arrow/right arrow: rotate around xy axis
2. up arrow/down arrow: rotate around yz axis
3. w/s: scale up and down the terrain map
4. escape to exit
