#pragma once

// C++ includes
#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>
using namespace std;

// C++ includes
#include <limits>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/chromosome.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/individual.hpp>
#include <metaheuristics/algorithms/problem.hpp>
#include <metaheuristics/random/random_number_generator.hpp>
#include <metaheuristics/misc/time.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random;
using namespace timing;

/**
 * @brief Implementation of the RKGA.
 * 
 * RKGA stands for Random-Key Genetic Algorithm.
 * 
 * In this algorithm the @ref genetic_algorithm::population is split into
 * two groups:
 * - The individuals within the range [0, @ref genetic_algorithm::N_MUTANT),
 * are mutant individuals.
 * -  The rest of @ref genetic_algorithm::population (range
 * [ @ref genetic_algorithm::N_MUTANT, @ref genetic_algorithm::pop_size)),
 * are crossover individuals.
 */
class rkga : public genetic_algorithm {
	private:
		/**
		 * @brief Implementation of the parents choice.
		 * 
		 * Choose two individuals at random from @ref genetic_algorithm::population.
		 * @e p1 and @e p2 are two indices within the range [0, @ref genetic_algorithm::pop_size).
		 */
		void get_two_parents(size_t& p1, size_t& p2);
		
	protected:
		
		// Sanity check
		/**
		 * @brief Makes sure that the different populations sizes are correct.
		 * 
		 * Checks that @ref genetic_algorithm::N_MUTANT < @ref genetic_algorithm::pop_size. Basically, a sanity check.
		 */
		bool are_set_sizes_correct() const;
		
	public:
		/// Default constructor
		rkga();
		/**
		 * @brief Constructor with parameters.
		 * @param p_size The total size of the population (see @ref genetic_algorithm::pop_size).
		 * @param m_size The amount of mutant individuals (see @ref genetic_algorithm::N_MUTANT).
		 * @param n_gen The number of generations (see @ref genetic_algorithm::N_GEN).
		 * @param chrom_size The size of each individual's chromosome (see @ref chrom_size).
		 * @param i_prob Inheritance probability (see @ref genetic_algorithm::in_prob).
		 * @param rand_gen The random number generator (see @ref genetic_algorithm::rng).
		 */
		rkga
		(
			size_t p_size,
			size_t m_size,
			size_t n_gen,
			size_t chrom_size,
			double i_prob,
			random_number_generator *rand_gen
		);
		/// Destructor.
		virtual ~rkga();
		
		/**
		 * @brief Executes the RKGA algorithm.
		 * 
		 * The RKGA executes the following steps:
		 * - Initialise the population with @ref genetic_algorithm::pop_size mutants
		 * - For as many generations as @ref genetic_algorithm::N_GEN :
		 * 		- Make the next generation @e Ng
		 * 		- Generate mutants in the range [0, @ref genetic_algorithm::N_MUTANT) in @e Ng
		 * 		- Generate the crossover individuals in the range [@ref genetic_algorithm::N_MUTANT, @ref genetic_algorithm::pop_size)
		 * 		  in @e Ng
		 * - Find the individual with the largest fit and decdode the chromosome.
		 *   Store the result in @e p. Store the cost of the solution in @e c.
		 * 
		 * @pre @p e is an empty instance of the problem, and @e c its cost.
		 * @post @e p is a solution to the problem with cost @e c.
		 */
		bool execute_algorithm(problem *p, double& c);
		
		/**
		 * @brief Prints the performance of the algorithm.
		 * 
		 * Prints to standard output a message with the following format:
		\verbatim
		RKGA algorithm performance:
			Total generation average:          DOUBLE s
			Average generation average:        DOUBLE s
			Total mutant generation time:      DOUBLE s
			Average mutant generation time:    DOUBLE s
			Total crossover generation time:   DOUBLE s
			Average crossover generation time: DOUBLE s
		\endverbatim
		 */
		void print_performance() const;
};

} // -- namespace algorithms
} // -- namespace metaheuristics

