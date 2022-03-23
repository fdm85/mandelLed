WS2812 Sandbox based on STM32                         {#mainpage}
============

Ws2812 offers flexibility regarding length/count of LEDs to be controlled.
This project is divided into two main parts.  
- Common files which implement:
  - low level driver for data output and converter of RGB values to raw data
  - memory abstraction to access LED strips
  - animation routines producing visual effects (called cyclic)
  - factory macros to ease allocation
  - math functions
  - led strip style
  - matrix style
  - generic development helper 
- Target dependent realization
  - Instantiation of (multiple) LED strips
  - mapping of IO 
  - (remote) control of to be used animations
  - memory mapping