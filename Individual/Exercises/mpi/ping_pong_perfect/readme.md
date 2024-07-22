 # Ejercicio 59 [ping_pong_perfect]
 
Un ejercicio clásico de paso de mensajes es simular un juego de tenis de mesa (ping-pong) entre dos procesos. Uno lanza la bola al otro, quien la recibe y la regresa al primero, y así sucesivamente. Los dos jugadores son incansables y nunca pierden un servicio. Haga que su programa simule este comportamiento. Si su programa es invocado con una cantidad distinta a dos jugadores, debe reportar un mensaje de error y finalizar.

Haga que cada proceso imprima en la salida estándar un mensaje cuando hace un servicio. Puede permitir que el usuario especifique un segundo argumento de línea de comandos para establecer una espera en milisegundos que tarda un jugador en servir desde que recibe la bola. Esto puede ayudar a hacer más legible la salida disminuyendo el indeterminismo. El siguiente podría ser un ejemplo de ejecución hipotético.

    bin/ping_pong_perfect 2 500
    0 serves
    1 serves
    0 serves
    1 serves
    ^C