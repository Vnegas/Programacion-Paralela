 # Ejercicio 63 [hybrid_distr_wtime]

Modifique el programa del rango del [Ejercicio 61 hybrid_distr_stdin](../hybrid_distr_stdin/readme.md) para que cada proceso reporte en la salida estándar la cantidad de segundos en que tarda su ejecución. Mida la duración desde que inicializa el ambiente de ejecución de MPI (`MPI_Init`) hasta inmediatamente antes de finalizarlo (`MPI_Finalize`). Un ejemplo de salida podría ser el siguiente.

    $ mpiexec -n 2 -f hosts.txt ./hybrid_distr_wtime 3 20
    hostname1:0: range [3, 12[ size 9 in 0.000123s
      hostname1:0.0: range [3,6[ size 3
      hostname1:0.1: range [6,9[ size 3
      hostname1:0.2: range [9,12[ size 3
    hostname2:1: range [12, 20[ size 8 in 0.000342s
      hostname2:1.0: range [12,15[ size 3
      hostname2:1.1: range [15,18[ size 3
      hostname2:1.2: range [18,20[ size 2

¿Cuál de las dos formas de especificar el rango consigue menores tiempos de ejecución: en argumento de línea de comandos o la entrada estándar?

El programa consigue mejor o menor tiempo de ejecución si los rangos se especifican por medio de argumento de línea de comandos. Esto puede deberse al proceso de `send()` y `receive()` que retrasan la ejecución.
