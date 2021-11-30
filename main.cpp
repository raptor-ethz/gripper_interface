#include <stdio.h>

#include <iostream>
int main() {
  FILE *file;
  // Opening device file

  int getnum;

  while (true) {
    file = fopen("/dev/ttyACM0", "w");
    std::cout << ">>" << std::endl;
    std::cin >> getnum;
    fprintf(file, "%d", getnum);  // Writing to the file
    fclose(file);
  }
}