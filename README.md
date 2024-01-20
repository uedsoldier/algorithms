Funciones y macros correspondientes a algoritmos de comprobación de errores, sumas de verificación, cifrado y codificación para aplicaciones orientadas a microcontroladores.
En caso de que este código sea necesitado, se debe hacer una copia de los archovos requeridos en vez de modificar el repositorio orginal.

# Módulos

* checksum8: Sumas de verificación de 8 bits.
* CRC: Implementación de verificación de redundancia cíclica (CRC) de 8, 16 y 32 bits, con distintos polinomios generadores e implementaciones.
* XTEA: Implementación del algoritmo de cifrado Extended Tiny Encryption Algorithm, para aplicaciones embebidas de poca memoria y poder computacional.
* BASE64: Codificación (hash) de datos binarios en base 64, para su uso en aplicaciones como correo electrónico y otras más.
* AES:  Implementación del algoritmo de cifrado simétrico AES en sus variantes ECB y CBC, con claves de 128,192 y 256 bits.

# Uso de CMake para generar binarios de pruebas
* Para Windows:
``` cmake -S . -B build/ -G "MinGW Makefiles"```
Desde la carpeta build/: 
``` mingw32-make -j<n>```
* Para Linux:
``` cmake -S . -B build/ -G "MinGW Makefiles"```
Desde la carpeta build/: 
``` make -j<n>```

Donde 'n' representa el número de procesos en paralelo para completar el comando 'make'

# Pruebas de funcionamiento
Dentro de la carpeta ```build/``` de cada módulo de pruebas, se genera un ejecutable con nombre ```test```
* BASE64
    * Parámetros: 
        * Cadena de entrada
    * Salida:
        * Codificación equivalente en BASE64
* checksum8
    * Parámetros: 
        * Cadena de entrada
    * Salida:
        * Checksum8 XOR
        * Checkum8 módulo 256
        * Checksum8 complemento a 2
* CRC
    * Parámetros: 
        * Cadena de entrada
    * Salida:
        * Diversas salidas de CRC de 8, 16 y 32 bits
* XTEA
    * Parámetros: 
        * Cadena de entrada
        * Cadena de llave de algoritmo
        * Bandera CBC/EBC (0 o 1)
        * Número de rondas (1,2,4,8,16,32 o 64)
    * Salida:
        * Texto cifrado y descifrado bajo los parámetros indicados

# Referencias
E inspiración:
* AES: 
    * https://github.com/kellencataldo/aes_lib/tree/master
    * https://github.com/kokke/tiny-AES-c/tree/master