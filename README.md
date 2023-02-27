# Arduino-EEPROM-Programmers
This is a repository for the arduino code used to program the W27C512-45Z EEPROM chips for the ADA-0 project. ADA-0 is a computer built from scratch based on TO-92 transistors, still in initial development stages.

Each .ino file contains the code used to program a different set of EEPROMs that compose a different section of the output module of ADA-0 (and also the code for erasing them). The integers, float and HEX sections of the output module work with 7-segment displays, while the "LED matrix" section uses a 5x7 LED matrix.

Both EEPROM_hex.ino and EEPROM_ledMatrix.ino are still being developed. All schematics will be added to this repository in due time. Feel free to contact me if I can help you in any way.
