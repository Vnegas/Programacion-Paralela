 # Tarea03: pthread_optimized

Programa que encuentra mediante fuerza bruta la contraseña para desencriptar los archivos comprimidos dentro de un archivo ZIP. El programa no extrae los archivos, simplemente encuentra e imprime la contraseña en la salida estándar. Para un mayor desempeño, se implementa concurrencia. Los hilos se dividen el trabajo de encontrar la posible contraseña para cada archivo.

El programa recibe como parámetro el número de hilos que se van a utilizar en la ejecución, si no se provee utiliza el número máximo de hilos del equipo. El equipo se refiere a los consumidores que tratarán de abrir el archivo zip y verificar la contraseña. Mientras que el productor, solo uno, encuentra la siguiente posible contraseña.

El programa lee de la entrada estándar un lote de trabajo, como el presentado en el siguiente ejemplo:

> Ejemplo de entrada:

    0123456789
    5

    tests/zip_05/f01.zip
    tests/zip_05/f23.zip
    tests/zip_05/f09.zip

> Ejemplo de salida:

    tests/zip_05/f01.zip 00112
    tests/zip_05/f23.zip
    tests/zip_05/f09.zip 9209

Un lote consiste de lo siguiente:
1. La primera línea corresponde al alfabeto con que se escribió la o las contraseñas. Corresponde a un subconjunto de caracteres ASCII.
2. La cantidad máxima de caracteres que posee la contraseña. Corresponde a un límite para mantener la búsqueda finita.
3. Una línea en blanco de separación.
4. La cuarta y subsecuentes líneas corresponden a una lista de archivos ZIP a los que se les debe encontrar la contraseña. Cada línea contiene un nombre de archivo y una ruta relativa a la carpeta donde se encuentra su proyecto.

### Valor de retorno
---
En la salida estándar se imprime la ruta de cada archivo y la contraseña que lo desencripta separados por un espacio en blanco. 

Si el programa no pudo encontrar la contraseña, imprime sólo el nombre del archivo y un cambio de línea. 

Los archivos en la salida se imprimen en el mismo orden en que se encuentran en la entrada estándar.

Para una representación gráfica, se puede observar aquí [representacion_grafica](./design/readme.md)

### Creditos
---
Sebastian Venegas Brenes

Correo: sebastian.venegas@ucr.ac.cr
