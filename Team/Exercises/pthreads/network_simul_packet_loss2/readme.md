 # Ejercicio 25 [network_simul_packet_loss2]

Modifique su solución al `Ejercicio 24 [network_simul_packet_loss]` para que el hilo que pierde paquetes esté ubicado entre el productor y el repartidor, de tal forma, que el porcentaje de pérdida de paquetes afecte a todos los paquetes y no una fracción de los mensajes de red.

¿Cuántas líneas de código debió modificar para reflejar este cambio? ¿Cuántas de esas líneas fueron en la implementación de su (a) productor, (b) repartidor, (c) consumidor, (d) ensamblador, y (e) controlador de la simulación?

Tuve que modificar alrededor de 9 líneas de código.

- Productor(a) = 0 líneas modificadas
- Repartidor(b) = 1 líneas modificadas
- Consumidor(c) = 0 líneas modificadas
- Ensamblador(d) = 0 líneas modificadas
- Controlador de simulación(e) = 8 líneas modificadas