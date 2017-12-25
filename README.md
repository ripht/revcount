# revcount
MotoGP style revcounter. This sketch is the prototyping version for the Arduino Uno to drive an adafruit NeoPixel ring to serve as a 0-15000RPM rev counter with a white strobe shift indicator around 11.000RPM. The adafruit trinket version will follow. 

V1.0: updating the display was implemented by refreshing it entirely, this is not neccesary and ate up processing time and memory.
V2.0: the display part works, next up is to use an ISR to interface with the bike's ignition timing computer.
V3.0: ...
