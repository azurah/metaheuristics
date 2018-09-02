#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// C++ includes
#include <limits>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/genetic_algorithms/chromosome.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/individual.hpp>
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/algorithms/problem.hpp>
#include <metaheuristics/random/random_number_generator.hpp>
#include <metaheuristics/misc/time.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace timing;
using namespace structures;
using namespace random;

/**
 * @brief Abstract class to implement genetic algorithms.
 * 
 * In this context, a "mutant individual" is an individual whose
 * chromosome has been randomly generated.
 * 
 * This class need the following methods of class @ref problem implemented:
 * - @ref problem::decode
 * 
 */
class genetic_algorithm : public metaheuristic {
	private:
	protected:
		/// Total execution of the algorithm.
		double total_time;
		/**
		 * @brief Time needed to initialise the population.
		 * 
		 * Usually, this is just the time needed for allocating the
		 * memory, generating a random chromosome for each individual
		 * and evaluating the individuals.
		 */
		double initial_time;
		/// Time spent on doing the crossover of individuals.
		double crossover_time;
		/// Time spent in generating mutant individuals.
		double mutant_time;
		
		/// Size of the population.
		size_t pop_size;
		/// Size of the mutant population.
		size_t N_MUTANT;
		/// Number of generations of the population.
		size_t N_GEN;
		/// Size of the chromosomes of each individual.
		size_t chrom_size;
		/// Probability of inheritance.
		double in_prob;
		
		/**
		 * @brief Typedef for the population set.
		 * 
		 * Basically, a shorthand for the vector that contains elements
		 * of type @ref individual and uses an allocator specially for
		 * that class (see @ref structures::allocator_individual).
		 */
		typedef vector<individual, allocator_individual<individual> > population_set;
		/// The population of individuals.
		population_set population;
		
		// Information display functions
		/// 
		/**
		 * @brief Prints to standard output the current population.
		 * 
		 * Each line contains the index of the individual within the
		 * population (0,1,2,3,...), its fitness and its chromosome.
		 */
		void print_current_population() const;
		
		// Algorithm-related functions
		/// Fills the population with mutants and decodes them.
		void initialize_population(const problem *p);
		/**
		 * @brief The individuals within the range [A,B) are replaced with mutants.
		 * @param[in] p The problem for which a solution will be generated with each chromosome.
		 * @param[in] A Lower bound of interval.
		 * @param[in] B Upper bound of interval.
		 * @param[out] next_gen The next generation of individuals, created
		 * from @ref population. The individuals within the range [@e A,@e B)
		 * of this set are modified.
		 * @param[out] m At the end of the execution @e m equals @e B.
		 */
		void generate_mutants(const problem *p, size_t A, size_t B, population_set& next_gen, size_t& m);
		
		/**
		 * @brief Choose two parents to make a crossover individual.
		 * 
		 * With probability @ref in_prob, parent @e p1's gene is chosen
		 * over @e p2's gene when making a new individual.
		 * 
		 * @param[out] p1 First parent. This parent is chosen over @e p2
		 * with probability @ref in_prob.
		 * @param[out] p2 Second parent.
		 */
		virtual void get_two_parents(size_t& p1, size_t& p2) = 0;
		/**
		 * @brief Generates the crossover population.
		 * 
		 * @param[in] p The problem for which a solution will be generated
		 * with each chromosome.
		 * @param[out] next_gen The next generation of individuals, created
		 * from @ref population. The individuals within the range
		 * [m, @ref pop_size) are modified.
		 * @param[out] m At the end of the execution @e m equals @ref pop_size.
		 */
		void generate_crossovers(const problem *p, population_set& next_gen, size_t& m);
		
		/**
		 * @brief Evaluates an individual.
		 * 
		 * Creates a solution using method problem::decode and @e i's
		 * chromosome. Then sets @e i's fitness using the cost of that
		 * solution.
		 * 
		 * @param[in] p The problem for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[out] i Inidividual from which the solution is generated.
		 * @pre Inidividual @e i has its chromosome set.
		 * @post Inidividual @e i is given its fitness.
		 */
		void evaluate_individual(const problem *p, individual& i) const;
		/**
		 * @brief Generates a mutant individual.
		 * 
		 * Generates its random chromosome, and evaluates the individual
		 * (see @ref evaluate_individual(const problem*, individual&).
		 * @param[in] p The problem for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[out] i Inidividual from which the solution is generated.
		 * @post Inidividual @e i is given its chromosome and fitness.
		 */
		void generate_mutant(const problem *p, individual& i);
		
		/**
		 * @brief Makes the crossover of @e i-th and @e j-th individuals.
		 * 
		 * @e son is the result of making the crossover of the @e i-th
		 * and @e j-th individuals. With probability @ref in_prob,
		 * @e son will inherit @e i's gene.
		 * 
		 * @param[in] p The problem for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[in] i First parent.
		 * @param[in] j Second parent.
		 * @param[out] son The result of the crossover of @e i-th and @e j-th
		 * individuals.
		 */
		void crossover(const problem *p, size_t i, size_t j, individual& son);
	
	public:
		/// Default constructor.
		genetic_algorithm(); 
		/**
		 * @brief Constructor with parameters.
		 * @param population_size The total size of the population
		 * (see @ref pop_size).
		 * @param mutant_population_size The size of the mutant population
		 * (see @ref N_MUTANT).
		 * @param num_generations The number of generations the algorithm
		 * will make (see @ref N_GEN).
		 * @param chrom_size The size of each inidivual's chromosome
		 * (see @ref chrom_size).
		 * @param inheritance_probability Inherith with such probability
		 * the first parent's gene (see @ref in_prob).
		 * @param rng The random number generator (see metaheuristic::rng).
		 */
		genetic_algorithm
		(
			size_t population_size,
			size_t mutant_population_size,
			size_t num_generations,
			size_t chrom_size,
			double inheritance_probability,
			random_number_generator *rng
		);
		/// Destructor.
		virtual ~genetic_algorithm();
		
		/**
		 * @brief Resets the algorithm to its initial state.
		 * 
		 * For example, it sets the variables @ref total_time,
		 * @ref initial_time, @ref crossover_time, @ref mutant_time.
		 */
		virtual void reset_algorithm();
		
		/// Returns a constant reference to the individual with largest fitness.
		virtual const individual& get_best_individual() const;
		/**
		 * @brief Returns a constant reference to the i-th individual.
		 * @pre @e i < @ref pop_size.
		 */
		const individual& get_individual(size_t i) const;
		
		/**
		 * @brief Returns the size of the population.
		 * @returns Returns the value of @ref pop_size.
		 */
		size_t population_size() const;
		
		/**
		 * @brief Executes a genetic algorithm.
		 * @param[out] p The solution obtained using the genetic algorithm.
		 * @param[out] c The cost of solution obtained using the genetic algorithm.
		 * @pre @e p contains an empty instance of the problem.
		 * @e c is the cost of @e p (the cost of an empty solution). 
		 */
		virtual bool execute_algorithm(problem *p, double& c) = 0;
		
		/// Prints a summary of the performance of the algorithm
		virtual void print_performance() const = 0;
};

} // -- namespace algorithms
} // -- namespace metaheuristics

