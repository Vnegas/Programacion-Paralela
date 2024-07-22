 # Ejercicio 54 [mpi_wrapper]
 
Escriba una clase reutilizable Mpi en C++ que encapsule funcionalidad repetitiva del estándar MPI y provea un buen manejo de errores. Dado que usará plantillas, su clase estará en un archivo Mpi.hpp. En ejercicios posteriores usted incrementará y reutilizará esta clase.

La clase debe tener al menos los atributos indicados en el pseudocódigo propuesto para ejecución distribuida (Pseudocódigo de paso de mensajes), esto es: número de proceso (int), cantidad de procesos en el equipo (int), y nombre de la máquina donde corre el proceso (std::string).

Provea un constructor que reciba referencias a los argumentos de línea de comandos, inicialice el ambiente de ejecución de MPI, y llame funciones de MPI para inicializar los atributos de la clase. Lance una instancia de std::runtime_error en el constructor si no puede inicializar el ambiente de MPI u obtener el valor para un atributo. Provea un mensaje descriptivo en cada excepción. Provea un destructor que termine la ejecución del ambiente MPI, pero no lance excepciones en caso de fallo.

Para cada atributo de la clase provea un método get en línea (inline), pero no provea métodos set. Provea además un método rank() para obtener el el número de proceso, y un método size() para obtener la cantidad de procesos en el equipo.

Documente la clase, atributos, y métodos con Doxygen. Agregue a la carpeta un archivo main.cpp con el siguiente código que debería producir el mismo resultado que el Ejemplo 43. Pruebe que al ejecutarlo con varios procesos, estos saluden de forma indeterminística.

    int main(int argc, char* argv[]) {
      try {
        Mpi mpi(argc, argv);
        std::cout << "Hello from main thread of process " << mpi.getProcessNumber()
          << " of " << mpi.getProcessCount() << " on " << mpi.getHostname()
          << std::endl;
      } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << std::endl;
      }
    }

 # Ejercicio 55 [mpi_error]

Implemente una clase para transportar potenciales errores que puedan surgir al usar la tecnología MPI y que necesitará en ejercicios siguientes.

Cree un archivo `MpiError.hpp` dentro de la carpeta `mpi_wrapper/.` Cree una clase MpiError que herede de std::runtime_error. Declare los tres constructores que están en el siguiente fragmento de código. Necesitará hacer un forward declaration de la clase Mpi.

    explicit MpiError(const std::string& message);
    MpiError(const std::string& message, const Mpi& mpi);
    MpiError(const std::string& message, const Mpi& mpi, const int threadNumber);

En un archivo `MpiError.cpp` implemente los tres constructores. Cada uno debe invocar al constructor de std::runtime_error y proveerle un texto cuyo formato es el siguiente para cada constructor (las líneas corresponden corresponden a los constructores del código previo):

    message
    hostname:process_number: mesasge
    hostname:process_number.thread_number: mesasge

Modifique la implementación de la clase Mpi para que cuando una función de la biblioteca MPI retorne un código de error, su clase lance una instancia de `MpiError`. Pruebe su código con varios procesos. Modifique temporalmente su código para provocar o lanzar una excepción y verificar que el texto con formato de la excepción se imprima en el error estándar.

 # Ejercicio 56 [send_recv_ord_sem_wrap]

Modifique su clase Mpi en la carpeta `mpi_wrapper/` (no cree una nueva) para que pueda enviar y recibir mensajes punto a punto con métodos `send()` y `receive()`. La interfaz de estos y futuros métodos de esta clase serán similares al pseudocódigo propuesto para distribución (Pseudocódigo de paso de mensajes).

Naturalmente es deseable que su clase Mpi sea capaz de enviar y recibir valores de todos los tipos de datos primitivos soportados por MPI. Sin embargo no debe introducir redundancia de código, es decir, evitar algo como:

    class Mpi {
      ...
    public:
      // MPI_CHAR, MPI_SIGNED_CHAR
      void send(const char& ch, int toProcess, int tag = 0);
      // MPI_UNSIGNED_CHAR, MPI_BYTE
      void send(const unsigned char& ch, int toProcess, int tag = 0);
      // MPI_SHORT...

    public:
      // MPI_CHAR, MPI_SIGNED_CHAR
      void receive(char& ch, int fromProcess, int tag = MPI_ANY_SOURCE);
      // MPI_UNSIGNED_CHAR, MPI_BYTE
      void receive(unsigned char& ch, int fromProcess, int tag = MPI_ANY_SOURCE);
      ...
    };

Para evitar esta redundancia de código, utilice el paradigma de programación genérica. Cree plantillas de subrutinas. La plantilla recibe por parámetro el tipo de datos genérico a enviar o recibir por MPI. Sin embargo, la implementación de estos métodos tendrá que invocar funciones de la biblioteca de MPI que no son genéricas, sino que requieren constantes enteras acordes al tipo de datos. Puede usar las siguientes subrutinas que mapean los tipos de datos primitivos de C++ a constantes enteras que MPI requiere:

    public:
      static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
      static inline MPI_Datatype map(char) { return MPI_CHAR; }
      static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; }
      static inline MPI_Datatype map(short) { return MPI_SHORT; }
      static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; }
      static inline MPI_Datatype map(int) { return MPI_INT; }
      static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
      static inline MPI_Datatype map(long) { return MPI_LONG; }
      static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
      static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
      static inline MPI_Datatype map(unsigned long long) { return MPI_UNSIGNED_LONG_LONG; }
      static inline MPI_Datatype map(float) { return MPI_FLOAT; }
      static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
      static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }

A modo de ejemplo, con los métodos anteriores, la expresión map(0.0) se evaluará por MPI_DOUBLE, y la expresión map(double()) también se evaluará por MPI_DOUBLE. Es decir, con un valor value cualquiera del tipo de datos typename DataType se puede obtener su constante de MPI correspondiente con la expresión map(value) ó map(DataType()). Provea entonces dos plantillas en su clase de acuerdo al siguiente interfaz en pseudocódigo para envío y recepción de valores escalares:

    send(value, toProcess, tag=0)
    receive(&value, fromProcess, tag=any_tag)

La primera plantilla servirá para generar métodos que reciben un valor (referencia constante) del tipo de datos genérico, y lo envía hacia el proceso destino usando `MPI_Send()`. La segunda plantilla recibe una referencia no constante de una variable y en ella escribe un valor recibido por la red proveniente del proceso fromProcess con MPI_Recv(). Con estos métodos el ejemplo Ejemplo 46 podrá reducirse al siguiente código que es mucho más claro y fácil de mantener:

    #include "../mpi_wrapper/Mpi.hpp"

    int main(int argc, char* argv[]) {
      try {
        Mpi mpi(argc, argv);
        const int rank = mpi.getProcessNumber();
        const int count = mpi.getProcessCount();
        const int previous = (count + rank - 1) % count;
        const int next = (rank + 1) % count;

        bool can_print = true;
        if (rank > 0) {
          mpi.receive(can_print, previous);
        }
        std::cout << "Hello from main thread of process " << rank << " of "
          << count << " on " << mpi.getHostname() << std::endl;
        mpi.send(can_print, next);
      } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << std::endl;
      }
    }

Copie el código anterior a un archivo `main.cpp` en una carpeta `send_recv_ord_sem_wrap/`, asegúrese de que compila y produce los resultados esperados.

 # Ejercicio 57 [send_recv_ord_itm_wrap]

Las funciones de paso de mensajes de MPI son orientadas a arreglos. Todas ellas reciben la dirección de memoria (puntero) donde inicia un arreglo en la memoria. Si la operación es un *send*, se envía la cantidad de elementos a partir de esa dirección al proceso destino. Si la operación es un *receive*, se escribe a partir de esta dirección los elementos que se recibieron por la red hasta un máximo indicado por una cantidad de elementos. Dado que MPI está diseñado pensando en arreglos, para enviar o recibir un único elemento hay que enviar su dirección de memoria e indicar que sólo 1 elemento conforma el arreglo.

Hasta el momento su implementación de la clase *wrapper* de MPI sólo permite enviar valores escalares. En este ejercicio, modifique la clase Mpi en la carpeta `mpi_wrapper/` para proveer variaciones de los métodos genéricos `send()` y `receive()` que puedan enviar y recibir arreglos de valores. Estas variaciones están indicadas en el siguiente pseudocódigo, donde Type es un tipo de datos genérico.

    /// Send a scalar value to another process
    send(const Type& value, toProcess, tag = 0)
    /// Send an array of count values to another process
    send(const Type* values, count, toProcess, tag = 0)
    /// Send an array of values to another process
    send(const std::vector<Type>& values, toProcess, tag = 0)
    /// Send a text to another process
    send(const std::string& text, toProcess, tag = 0)

    /// Wait until it receives a scalar value from other process
    receive(Type& value, fromProcess = any_process, tag = any_tag)
    /// Wait until it receives at most capacity values from another process
    receive(Type* values, capacity, fromProcess = any_process, tag = any_tag)
    /// Wait until it receives at most capacity values from another process
    receive(std::vector<Type>& values, capacity, fromProcess = any_process, tag = any_tag)
    /// Wait until it receives a text of at most length chars from another process
    receive(std::string& text, capacity, fromProcess = any_process, tag = any_tag)

En la carpeta send_recv_ord_itm_wrap/ provea un archivo main.cpp que use los métodos send() y receive() de textos para lograr la funcionalidad del ejemplo de imprimir en orden a través de un intermediario.
