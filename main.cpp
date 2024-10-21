#include <iostream>
#include <fstream>
#include "bloom_filter.hpp"

void process_strings(const std::string &file_path, compressible_bloom_filter &table)
{
  std::fstream s{file_path, std::ios::in};

  if (!s.is_open())
  {
    std::cerr << "Error: Could not open the file: " << file_path << std::endl;
    return;
  }

  std::string new_line;

  while (std::getline(s, new_line))
  {
    const char operation = new_line[0];
    new_line = new_line.substr(2);

    if (operation == '+')
    {
      table.insert(new_line);
    }
    else
    {
      if (table.contains(new_line))
      {
        std::cout << new_line << " - yes" << std::endl;
      }
      else
      {
        std::cout << new_line << " - no" << std::endl;
      }
    }
  }
}

int main(int argc, char *argv[])
{
  unsigned int random_seed = 0xA57EC3B2;

  const double desired_probability_of_false_positive = 0.01;

  bloom_parameters parameters;
  parameters.projected_element_count = 1000000;
  parameters.false_positive_probability = desired_probability_of_false_positive;
  parameters.random_seed = random_seed++;
  parameters.maximum_number_of_hashes = 7;

  if (!parameters)
  {
    std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
    return 1;
  }

  parameters.compute_optimal_parameters();

  std::string file_path = argv[1];
  compressible_bloom_filter filter(parameters);

  process_strings(file_path, filter);

  return 0;
}
