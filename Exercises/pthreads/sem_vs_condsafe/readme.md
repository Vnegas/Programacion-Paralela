 # Ejercicio 16 [sem_vs_condsafe]

Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite. Reporte en su `readme.md` las tres duraciones de cada solución. Tome la menor duración de las tres corridas de cada versión. ¿Cuál solución realiza su trabajo en menor tiempo? Discuta en máximo un párrafo cuál podría ser la causa de este comportamiento.

### semaphor

1. Execution time: 5.366497866s
2. Execution time: 5.542087740s
3. Execution time: 5.649323700s

### conditionally safe

1. Execution time: 2.417844599s
2. Execution time: 2.603042761s
3. Execution time: 2.378811645s

---

La solucion que no implica control de concurrencia (conditionally safe), realiza la ejecucion en un menor tiempo bastante significativo.

Esto se puede deber al hecho que los hilos no tienen que esperar para realizar su trabajo, sino que todo se realiza simultaneamente.