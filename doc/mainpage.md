WS2812 Sandbox based on STM32                         {#mainpage}
============

Ws2812 offers flexibility regarding length/count of LEDs to be controlled.  
<a href="https://www.mikrocontroller.net/articles/WS2812_Ansteuerung">See here for details (in German)</a> 
This project is divided into two main parts.  
- Common files which implement:
  - low level driver for data output and converter of RGB values to raw data @ref Led_Data
  - memory abstraction to access LED strips @ref AccessAbstraction
  - animation routines producing visual effects (called cyclic) @ref Animations
  - factory macros to ease allocation @ref MemoryAbstraction
  - math functions @ref Math
  - led strip style @ref Animations
  - matrix style @ref Matrix
  - generic development helper @ref Dev_Helper
- Target dependent realization
  - Instantiation of (multiple) LED strips @ref LedCreator
  - mapping of IO @ref PeripheralInterface
  - (remote) control of to be used animations @ref ComApi
  - memory mapping @ref Linker_Map