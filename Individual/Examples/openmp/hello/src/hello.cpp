#include <iostream>

int main() {
  std::cout << std::endl;
  #pragma omp parallel
  {
    {
    #pragma omp critical
      std::cout << "Hello from secondary thread" << std::endl;
    }
    
  }
  #pragma omp parallel
  std::cout << "Bye\n";
}
