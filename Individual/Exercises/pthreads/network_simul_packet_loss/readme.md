 # Ejercicio 24 [network_simul_packet_loss]

Modifique el código de la simulación de red para introducir pérdida de paquetes. Reciba un parámetro más en línea de comandos que corresponde a la probabilidad de perder paquetes, como un porcentaje en precisión flotante. Implemente un hilo ensamblador que es uno de los destinos del repartidor.

Por cada paquete que este hilo ensamblador recibe, se genera un número flotante pseudoaleatorio entre 0 y 100. Si este número generado es menor que la probabilidad de pérdida de paquetes, el hilo descarta el paquete, de lo contrario, modifica al azar el destino del paquete, y lo pone de regreso en la cola entre el productor y el repartidor.