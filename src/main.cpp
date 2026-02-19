#include <atomic>
#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

void countPoints(const long radius, const long seed, const size_t tries_per_th, std::atomic<size_t> &tries_inside)
{
  std::mt19937                           mt_gen(seed);
  std::uniform_real_distribution<double> u_dist(-radius, radius);
  size_t                                 loc_tries_inside = 0;
  for (size_t i = 0; i < tries_per_th; ++i)
  {
    double x = u_dist(mt_gen);
    double y = u_dist(mt_gen);
    if (x * x + y * y <= radius * radius)
    {
      ++loc_tries_inside;
    }
  }
  tries_inside += loc_tries_inside;
}

void computeSqr(const long tries, const long seed)
{
  double radius    = 0;
  size_t n_threads = 0;
  while (std::cin >> radius >> n_threads)
  {
    if (radius <= 0 || n_threads <= 0)
    {
      std::cerr << "Incorrect radius or number of threads\n";
      return;
    }
    std::atomic<size_t>      tries_inside = 0;
    const size_t             tries_per_th = tries / n_threads;
    const size_t             tries_add    = tries % n_threads;
    std::vector<std::thread> threads(n_threads);
    auto                     start = std::chrono::system_clock::now();
    for (size_t i = 0; i < n_threads; ++i)
    {
      threads[i] =
        std::thread(countPoints, radius, seed + i, tries_per_th + (i < tries_add ? 1 : 0), std::ref(tries_inside));
    }
    for (auto &th : threads)
    {
      th.join();
    }
    double sq      = 4.0 * (static_cast<double>(tries_inside) / static_cast<double>(tries)) * radius * radius;
    auto   finish  = std::chrono::system_clock::now();
    double elapsed = std::chrono::duration<double, std::milli>(finish - start).count();
    std::cout << std::fixed << std::setprecision(3) << elapsed << " ";
    std::cout << std::fixed << std::setprecision(3) << sq << std::endl;
  }
}

int main(int argc, char **argv)
{
  if (argc < 2 || argc > 3)
  {
    std::cerr << "Incorrect amount of argument\n";
    return 1;
  }
  char *p_end{};
  errno            = 0;
  const long tries = std::strtol(argv[1], &p_end, 10);
  if (errno == ERANGE || tries <= 0 || *p_end != '\0')
  {
    std::cerr << "Range error: 1 argument\n";
    return 1;
  }
  errno           = 0;
  const long seed = argc == 3 ? std::strtol(argv[2], &p_end, 10) : 0;
  if (errno == ERANGE || seed < 0 || *p_end != '\0')
  {
    std::cerr << "Range error: 2 argument\n";
    return 1;
  }
  computeSqr(tries, seed);
  return 0;
}
