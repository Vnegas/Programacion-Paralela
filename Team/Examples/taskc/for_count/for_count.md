# for_count

> 1. ¿Cuál es el valor más grande que la variable compartida count podría llegar a obtener?

10000. Ocurre cuando los 100 threads son ejecutados en secuencia, por ejemplo, cuando se tiene una sola CPU. Cada thread incrementa en 100 la variable compartida, que alcanzará el valor $100 \cdot 100 = 10000$

> 2. ¿Cuál es el menor valor que la variable compartida count podría llegar a obtener?

100. Ocurre cuando:

1. A cada thread se le da CPU para ejecutar una instrucción y luego se pasa al siguiente. De esta forma, en cada iteración todos los threads leen el mismo valor en la variable `temp`, y todos escriben 100 veces el mismo valor incrementado en uno en `count` antes de pasar a la próxima iteración.

2. Cuando un thread toma el valor 0 en `temp` y es enviado a dormir mientras todos los demás ejecutan sus ciclos. Cuando el thread es despertado, incrementará la variable `count` desde 0 hasta 100.
