 # Ejercicio 23 [prod_cons_unbound_cpp]
 
Traslade su solución de Ejemplo 10 del lenguaje de programación C a C++ para que aproveche los paradigmas de programación genérica y orientada a objetos. Tome en consideración las siguientes sugerencias:

Cambie la tecnología procedimental Pthreads, por los hilos provistos en la biblioteca estándar de C++. Aproveche el código que realizó en Ejercicio 11 [std_thread_cpp].

1. Cree una cola genérica que sea thread-safe, y por lo tanto, protegida por mecanismos de control de concurrencia de C++. Use contenedores de la biblioteca estándar de plantillas.

2. Cree una clase base abstracta para los hilos de ejecución.

3. Cree una plantilla base abstracta para los productores. Haga que herede de la clase abstracta de hilo de ejecución.

4. Cree una plantilla base abstracta para los consumidores, de forma similar a los productores.

5. Herede de las abstracciones anteriores, su productor y consumidor para la simulación.

6. Cambie el registro y funciones de la simulación por una clase. Use el productor y consumidor que adaptó en el punto anterior en la implementación de su simulación.
