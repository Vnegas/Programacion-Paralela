 # Ejercicio 21 [prod_cons_unbound_produce2]
 
En los diseños de Ejemplo 10 se proveyó una versión produce1() que usa variables contadoras y exclusión mutua para deneter a los productores. No se proveyeron versiones produce2() y produce3().

Se creó la versión produce2() que utiliza control de concurrencia en lugar de variables contadoras con exclusión mutua para detener a los productores. Usa la función try_wait(semaphore) que retorna false si el semáforo está en 0 o negativo en lugar de bloquear al hilo. Si el semáforo está en positivo, funciona igual que wait(semaphore).

Implementa la variante produce2() en Pthreads junto con consume1().