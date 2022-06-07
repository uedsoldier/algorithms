Funciones y macros correspondientes a algoritmos de comprobación de errores, sumas de verificación, cifrado y codificación para aplicaciones orientadas a microcontroladores.

# Módulos

* checksum_8: Sumas de verificación de 8 bits.
* CRC: Implementación de verificación de redundancia cíclica (CRC) de 8, 16 y 32 bits, con distintos polinomios generadores e implementaciones.
* XTEA: Implementación del algoritmo de cifrado Extended Tiny Encryption Algorithm, para aplicaciones embebidas de poca memoria y poder computacional.
* BASE64: Codificación de datos binarios en base 64, para su uso en aplicaciones como correo electrónico y otras más.

# Uso de CMake
* Para Windows:
``` cmake -S . -B build/ -G "MinGW Makefiles"```
Desde la carpeta build/: 
``` mingw32-make ```
* Para Linux:
``` cmake -S . -B build/ -G "MinGW Makefiles"```
Desde la carpeta build/: 
``` make ```