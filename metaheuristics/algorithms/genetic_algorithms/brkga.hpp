#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/chromosome.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/individual.hpp>
#include <metaheuristics/algorithms/problem.hpp>
#include <metaheuristics/random/random_number_generator.hpp>
#include <metaheuristics/misc/timing.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random;
using namespace timing;

/**
 * @brief Implementation of the BRKGA.
 * 
 * BRKGA stands for Biased Random-Key Genetic Algorithm.
 * 
 * In this algorithm the @ref genetic_algorithm::population is split into
 * three groups. The individuals within the range [0, @ref N_ELITE), are
 * elite individuals, those in
 * [ @ref N_ELITE, @ref N_ELITE + @ref genetic_algorithm::N_MUTANT) are
 * mutant individuals. The rest of @ref genetic_algorithm::population
 * (range [ @ref N_ELITE + @ref genetic_algorithm::N_MUTANT, @ref genetic_algorithm::pop_size)),
 * are crossover individuals.
 */
class brkga : public genetic_algorithm {
	protected:
		/// Execution time spent in copying elite individuals.
		double elite_copying_time;
		/// Amount of elite individuals in the population.
		size_t N_ELITE;
		/**
		 * @brief Set of elite individuals of the population.
		 * 
		 * Stored as pairs of (fitness, index), where index points to
		 * a position in @ref genetic_algorithm::population.
		 */
		vector<pair<double, size_t> > elite_set;
		
		// Information display functions
		/// Display to standard output the elite individuals
		void print_elite_set() const;
		
		// Algorithm-related functions
		/**
		 * @brief Implementation of the parents choice.
		 * @param[out] p1 Contains an index to an elite individual.
		 * @param[out] p2 Contains an index to a non-elite individual.
		 */
		void get_two_parents(size_t& p1, size_t& p2);
		/**
		 * @brief Copy the elite individuals.
		 * 
		 * Copy the elite individuals in @ref genetic_algorithm::population to @e next_gen,
		 * within the range [ @e m, @e m + @ref N_ELITE).
		 * 
		 * @param[in] p An empty instance of the problem.
		 * @param[out] next_gen A population set with the elite individuals
		 * from @ref genetic_algorithm::population in the range [ @e m, @e m + @ref N_ELITE).
		 * @param[out] m Position
		 * @pre @e m < @ref genetic_algorithm::pop_size
		 */
		void copy_elite_individuals(const problem *p, population_set& next_gen, size_t& m);
		/**
		 * @brief Returns whether the @e i-th individual is an elite individual or not.
		 * @pre @e i < @ref genetic_algorithm::pop_size.
		 */
		bool is_elite_individual(size_t idx) const;
		/**
		 * @brief Keeps track of the elite individuals.
		 * 
		 * "Sorts" the population by fitness and stores the index and the
		 * fitness of the @ref N_ELITE first individuals with largest
		 * fitness in @ref elite_set.
		 */
		void track_elite_individuals();
		
		// Sanity check
		/**
		 * @brief Makes sure that the different populations sizes are correct.
		 * 
		 * Checks that @ref genetic_algorithm::N_MUTANT < @ref genetic_algorithm::pop_size. Basically, a sanity check.
		 */
		bool are_set_sizes_correct() const;
		
	public:
		/// Constructor.
		brkga();
		/**
		 * @brief Constructor with parameters.
		 * @param p_size The total size of the population (see @ref genetic_algorithm::pop_size).
		 * @param m_size The amount of mutant individuals (see @ref genetic_algorithm::N_MUTANT).
		 * @param e_size The amount of elite individuals (see @ref N_ELITE).
		 * @param n_gen The number of generations (see @ref genetic_algorithm::N_GEN).
		 * @param chrom_size The size of each individual's chromosome (see @ref chrom_size).
		 * @param i_prob Inheritance probability (see @ref genetic_algorithm::in_prob).
		 * @param rand_gen The random number generator (see @ref genetic_algorithm::rng).
		 */
		brkga
		(
			size_t p_size,
			size_t m_size,
			size_t e_size,
			size_t n_gen,
			size_t chrom_size,
			double i_prob,
			random_number_generator *rand_gen
		);
		/// Destructor.
		virtual ~brkga();
		
		/**
		 * @brief Resets the algorithm to its initial state.
		 * 
		 * Sets to 0 the variable @ref elite_copying_time. Calls
		 * the function @ref genetic_algorithm::reset_genetic_algorithm().
		 */
		void reset_algorithm();
		
		/**
		 * @brief Returns the individual with the highest fitness.
		 * 
		 * This individual is retrieved from @ref elite_set.
		 */
		const individual& get_best_individual() const;
		
		/**
		 * @brief Executes the BRKGA algorithm.
		 * 
		 * The BRKGA executes the following steps:
		 * - Initialise the population with @ref genetic_algorithm::pop_size mutants
		 * - Track the @ref N_ELITE best individuals (see @ref elite_set)
		 * - For as many generations as @ref genetic_algorithm::N_GEN :
		 * 		- Make the next generation Ng.
		 * 		- Copy the best @ref N_ELITE individuals into @e Ng (in the range [0, @ref N_ELITE)).
		 * 		- Generate mutants in the range
		 * 			[ @ref N_ELITE, @ref N_ELITE + @ref genetic_algorithm::N_MUTANT)).
		 * 		- Generate the crossover individuals in the range
		 * 			[ @ref N_ELITE + @ref genetic_algorithm::N_MUTANT, @ref genetic_algorithm::pop_size).
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
			Total copying elite time:          DOUBLE s
			Average copying elite time:        DOUBLE s
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

