# Sebastian_Venegas

Repositorio destinado a los lenguajes C y C++, empleando el uso de la programación paralela y concurrente.

## Glosario

    1. Programación serial.
> Programación donde los problemas o tareas se ejecutan de manera secuencial.

    2. Programación concurrente.
> Es la programación que no es serial, en la cual se resuelven tareas de forma intercalada o de manera concurrente.

    3. Programación paralela.
> Donde las tareas se resuelven de manera simultanea.

    4. Concurrencia de tareas.
> Separación de asuntos o tareas, hilos realizando trabajos distintos.

    5. Paralelismo de datos.
> Una cantidad indefinida de hilos que hacen la misma tarea.

    6. Hilo de ejecución.
> Es un arreglo de valores que el OS carga en los registros de CPU y permiten ejecutar código en ella.

    7. Indeterminismo.
> No poder predecir el orden en el que se ejecutan o se van a ejecutar las instrucciones concurrentes.

    8. Memoria privada y compartida.
> Memoria privada: Memoria propia del hilo (todo hilo tiene este tipo de memoria). La puede modificar y alterar, y la memoria de los otros hilos no se afecta.

> Memoria compartida: Memoria que todos los hilos comparten y pueden acceder a ella. Si la memoria se altera, se altera para todos los hilos.

    9. Espera activa.
> Es un ciclo que hace a un hilo de ejecución esperar repetitivamente hasta que una condición se haga falsa.

    10. Condición de carrera.
> Modificación concurrente de memoria compartida.

    11. Control de concurrencia.
> Mecanismos que permiten controlar la concurrencia, controlando el tiempo de ejecución; el momento que un hilo ejecuta código y el tiempo que pasa durmiendo o en tiempo de espera. Estos mecanismos frenan la concurrencia.

    12. Seguridad condicional.
> Es evitar el control de la concurrencia, utilizando memoria (arreglos, colas, arboles).

    13. Exclusión mutua.
> Son herramientas para evitar que distintos hilos accedan al mismo tiempo a recursos. Se crean regiones críticas, donde se bloquean estos recursos y se vuelven privados temporalmente.

    14. Semáforo.
> Un semáforo es un entero con signo que se puede incrementar (signal) y decrementar (wait). Si el signo el negativo, significa que tiene que esperar para poder seguir ejecutando su tarea (el número indica cuantos están esperando) y si es positivo, indica que puede seguir ejecutando sin ninguna espera (el número indica cuantos hilos pueden pasar o seguir hasta que se "ponga en rojo" el semáforo).

    15. Barrera.
> Es un mecanismo de control de concurrencia que permite esperar a la cantidad de hilos que se necesiten para la siguiente ejecución.

    16. Variable de condición.
> Es un mecanismo de control de concurrencia muy versátil que permite a un hilo de ejecución esperar a que se cumpla una condición, y luego permite a todos los hilos de ejecución continuar. El efecto no se acumula, por lo que si no había ningún hilo esperando la señal de paso se pierde.

    17. Candado de lectura y escritura.
> Es un mecanismo de control de concurrencia que permite a varios hilos de ejecución leer una variable compartida, pero sólo permite a un hilo de ejecución escribir en la variable compartida. Se puede denominar como exlusión mutua categórica (categorical mutual exclusion) porque permite paralelismo de una categoría de hilos, pero excluye los de otra.

    18. Descomposición.
> Identificar las tareas (trabajos, procesamientos) y/o datos que se pueden realizar de forma independiente, para ejecutarlos de manera paralela. Se tiene que escoger unidades de trabajo, granularidad fina(tareas pequeñas) o granularidad gruesa (tareas grandes).

    19. Mapeo.
> Se realiza después de la descomposición. Consiste en asignar las unidades de trabajo (identificadas en la descomposición) a los ejecutantes.

    20. Incremento de velocidad.
> Capacidad de un sistema de incrementar la velocidad de ejecución de un programa. Se puede lograr con la implementación de programación paralela, programación concurrente, o ambas.

    21. Comunicación punto a punto entre procesos.
> Es la capacidad de un sistema de enviar mensajes entre procesos para lograr comunicación durante la ejecución. Mediante `send()` y `receive()`.

    22. Comunicación colectiva entre procesos.
> Permite que un proceso envie mensajes a todos los demás procesos ejecutandose.

    23. Reducción.
> Es la capacidad de aplicar una operación a todos los elementos de un arreglo, y devolver un único valor. Aplica una operación a todas las celdas que componen un arreglo, y lo `reducen` a un solo valor.
