# Diseño
## Diagrama de flujo de datos

![DataFlowDiagram](./img/DataFlowDiagramProyect1.2.drawio.svg)

En este diagrama de flujo de datos se muestra el recorrido desde que el usuario ingresa el número o grupo de números hasta que se resuelven sus respectivas sumas de goldbach o factorización prima, y son devueltas al usuario, primeramente el usuario hace una solicitud para algún grupo de números a los cuales se quiere que se le calcule lo deseado, dichas solicitudes son obtenidas por la clase httpServer y guardadas en un queue, dicha clase  maneja las solicitudes mediante Sockets los cuales son enviados del queue del servidor web hasta la clase httpConnectionHandler mediante el queue de consumo de dicha clase, httpConnectionHandler consume dichos sockets mediante el patrón productor-consumidor y los destruye para obtener el request y response y ponerlos enque para el dispatcher que enruta a su respectiva aplicación para su respectivo cálculo (sumas de goldbach, factorización prima), dichos cálculos se realizan en la clase GoldbachCalculator y son puestos en queue a al Packer que recibe cada número cálculado de manera separada y "empaquetados" para mostrárselos al usuario por la clase ShowMessage.