#include <iostream>

int main(int argc, char **argv)
{

  if (argc == 0 || argc > 2)
  {
    std::cerr << "Incorrect amount of argument\n";
    return 2;
  }
  errno            = 0;
  const long tries = std::strtol(argv[1], nullptr, 10);

  if (errno == ERANGE)
  {
    std::cerr << "Range error\n";
    return 2;
  }
  return 0;
}
