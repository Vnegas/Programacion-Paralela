# Proyecto 1

## 1.1 Descripción del problema y requerimientos

El problema por resolver consiste en una sencilla simulación por computadora que ayude a encontrar el momento de equilibro térmico de una lámina rectangular a la que se le inyecta calor constante por su borde. Para efectos de la simulación, se usará una matriz dividida en R filas y C columnas en donde las celdas son todas cuadradas, de ancho y alto h.

Cada celda de la matriz almacena una temperatura, la cual puede cambiar en el tiempo, después de transcurrido un tiempo Δt, la simulación pasará del instante k al instante k+1, y la temperatura en la lámina habrá variado (estado). Para simular esta relación de cambio de energía se usa una fórmula que toma en cuenta:

1. La energía que la celda i,j recibe de sus inmediaciones, y se calcula como la suma de las temperaturas de las cuatro vecinas
2. La energía que la celda pierde y se distribuye a sus cuatro celdas vecinas
3. La ganancia y pérdida de energía calculada en los dos puntos anteriores, la cual se divide entre el área de la celda
4. La cantidad de energía intercambiada en el periodo de tiempo depende de la calidad conductora de la lámina

La simulación continua hasta que se haya alcanzado el punto de equilibrio, lo cual ocurre cuando el calor se ha estabilizado en la lámina. Para esto se debe proveer un parámetro épsilon (ε) a la simulación, que representa el mínimo cambio de temperatura significativo en la lámina. En cada estado k se actualizan todas las celdas internas de la lámina. Si al menos una de las celdas internas tiene un cambio en su temperatura mayor a ε, indica que no se ha alcanzado aún el equilibrio y la simulación continúa con el siguiente estado k+1, de lo contrario se detiene y reporta los resultados de la simulación.

## 1.2 Manual de Usuario

Para correr el programa, hay que crear un ejecutable utilizando la terminal de línea de comandos de Linux siguiendo las siguientes instrucciones:

Abrir una terminal en la carpeta del proyecto y emitir el comando: `make clean && make`. Esto generar un ejecutable el cual se almacena en la carpeta `bin/`

Ejemplo:`bin/project2`

Una vez que se tiene un ejecutable construido se debe escribir en la terminal `bin/project2`, junto con un argumento obligatorio el cual indica la ubicación relativa del archivo de texto que contiene la información necesaria para simular la distribución de calor. Es decir, las placas, junto la duración de cada etapa Δt en segundos, la difusividad térmica α del material, las dimensiones h de las celdas y la sensibilidad del punto de equilibrio ε, en las mismas unidades de temperatura que se usaron en el archivo de la lámina, por último se recibe el número de hilos con los cuales se va a ejecutar la simulación, en el caso de no agregar un segundo argumento, se toma el número de núcleos que contiene la máquina.

Esto sería `bin/project2 jobs/job###/job###.txt $$` donde `###` indica el número del caso de pruebas y `$$` indica el número de hilos que se desea usar en la simulacion.

Ejemplo: `bin/project2 jobs/job001/job001.txt 8`

El comando anterior correria la simulacion con el trabajo de prueba 001, y con 8 hilos de ejecuccion.
Este argumento es opcional, en caso de que no se provea por el usuario se asumira la cantidad de nucleos del sistema ejecutando el programa, como la cantidad de hilos a usar.

## Créditos

Adrián Rivas Campos <adrian.rivas@ucr.ac.cr>

Antony Leonardo Picado Alvarado <antony.picado@ucr.ac.cr>

Dario Murillo Chaverri <dario.murillo@ucr.ac.cr>

Sebastián Venegas Brenes  <sebastian.venegas@ucr.ac.cr>
