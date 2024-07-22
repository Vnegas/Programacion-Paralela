 # Ejercicio 62 [hybrid_distr_file]

Modifique el programa del rango distribuido ([Ejercicio 61 [hybrid_distr_stdin]](../hybrid_distr_stdin/readme.md)) para que el usuario pueda espeficar un nombre de archivo en los argumentos de línea de comandos. Cada proceso abre y lee de este archivo los valores del rango. En caso de que el nombre del archivo no se especifique en los argumentos de línea de comandos, lea el rango en la entrada estándar. Si el nombre del archivo se provee pero no se puede leer o no contiene un rango, reporte un mensaje de error y finalice el programa.
