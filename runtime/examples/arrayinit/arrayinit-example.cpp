/*
 * Copyright (c) 2014-2020 Embedded Systems and Applications, TU Darmstadt.
 *
 * This file is part of TaPaSCo
 * (see https://github.com/esa-tu-darmstadt/tapasco).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <array>
#include <iostream>
#include <tapasco.hpp>

#define SZ 256
#define RUNS 25

typedef int32_t element_type;
constexpr int ARRAYINIT_ID = 11;

static uint64_t check_array(std::array<element_type, SZ> &arr) {
  unsigned int errs = 0;
  for (size_t i = 0; i < arr.size(); ++i) {
    if (arr[i] != (element_type)i) {
      std::cerr << "ERROR: Value at " << i << " is " << arr[i] << std::endl;
      ++errs;
    }
  }
  return errs;
}

int main(int argc, char **argv) {
  // initialize TaPaSCo
  tapasco::Tapasco tapasco;

  uint64_t errs = 0;

  // check arrayinit instance count
  uint64_t instances =
      tapasco_device_kernel_pe_count(tapasco.device(), ARRAYINIT_ID);
  std::cout << "Got " << instances << " arrayinit instances.";
  if (!instances) {
    std::cout << "Need at least one arrayinit instance to run.";
    exit(1);
  }

  for (int run = 0; run < RUNS; ++run) {
    // Generate array for arrayinit output
    std::array<element_type, SZ> result;
    result.fill(-1);
    // Wrap the array to be TaPaSCo compatible
    auto result_buffer_pointer = tapasco::makeWrappedPointer(
        result.data(), result.size() * sizeof(element_type));
    // Data will be copied back from the device only, no data will be moved to
    // the device
    auto result_buffer_out = tapasco::makeOutOnly(result_buffer_pointer);

    // Launch the job
    // Arrayinit takes only one parameter: The location of the array. It will
    // always initialize 256 Int`s.
    auto job = tapasco.launch(ARRAYINIT_ID, result_buffer_out);

    // Wait for job completion. Will block execution until the job is done.
    job();

    int iter_errs = check_array(result);
    errs += iter_errs;
    std::cout << "RUN " << run << " " << (iter_errs == 0 ? "OK" : "NOT OK")
              << std::endl;
  }

  if (!errs)
    std::cout << "Arrayinit finished without errors." << std::endl;
  else
    std::cerr << "Arrayinit finished wit errors." << std::endl;

  return errs;
}
