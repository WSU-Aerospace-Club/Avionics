# WSU Avionics
This project was written in C++ using Visual Studio with Visual Micro for a Fubarino SD control board.
# Installation
1. Install Aurdino IDE
2. Install Visual Studio 
3. Install Visual Micro
4. Download ChipKit board definitions in Visual Micro
5. Disable deep search in Visual Micro
6. Copy the MAVLINK directory to your Audrino library
7. 
  * in this file;
    * C:\Users\USER_NAME\AppData\Local\arduino15\packages\chipKIT\hardware\pic32\1.4.0\cores\pic32\chipKIT-application-32MX795F512.ld    
  * Change this line;
    * PROVIDE(_min_heap_size = 0x800) ;
  * to this;
    * PROVIDE(_min_heap_size = 0x8000) ;
  
You should now be able to compile the project
