 # Ejercicio 14 [hello_order_semaphor2]

Modifique su solución de Ejemplo 7 para inicializar todos los semáforos en 0. Luego haga que el hilo principal incremente el semáforo del hilo 0. ¿Produce el mismo resultado que su solución anterior?

Si, produce el mismo resultado de imprimir los hilos secundarios en orden. Sin embargo, quitandole el indeterminismo del hilo principal.

¿Qué pasa si el hilo principal incrementa el semáforo del hilo 0 después de hacer su saludo `Hello from main thread`?

Elimina el indeterminismo del hilo principal.
