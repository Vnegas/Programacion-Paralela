 # Ejercicio 32 [relay_race_order_effect]
 
¿Afecta el orden de creación de los equipos el resultado de la carrera de relevos? Modifique su solución de la Section 3.2.6 para crear los equipos en orden inverso y compare los resultados de las ejecuciones. Sugerencia: no duplique el código, sino que utilice metaprogramación (#ifdef…​) para escoger en tiempo de compilación el orden de creación de los equipos.

El resultado de la carrera de relevos, no se ve afectado mayor nivel por el orden de creación de los equipos. Sin embargo, el indeterminismo puede verse afectado a un bajo nivel.