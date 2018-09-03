#pragma once

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// metaheuristics includes
#include <metaheuristics/algorithms/genetic_algorithms/chromosome.hpp>
#include <metaheuristics/algorithms/infeasible_exception.hpp>
#include <metaheuristics/random/random_generator.hpp>

namespace metaheuristics {
namespace structures {

using namespace random;

/**
 * @brief Local Search Policy.
 * 
 * Policies when finding better solutions in the neighbourhood of
 * a problem:
 * - First Improvement: stop searching when a better solution than
 * the current has been found.
 * - Best Improvement: search all the neighbourhood and keep the best.
 */
enum local_search_policy {
	First_Improvement = false,
	Best_Improvement = true
};

/**
 * @brief Definition of the problem to be solved.
 * 
 * This interface defines several functions to handle a problem's
 * solution.
 * 
 * Consits of basic functions like copying, freeing, evaluating,
 * and simple construction of solutions. Also, it defines functions
 * for debugging, like sanity checks, printing, ...
 */
class problem {
	protected:
		/**
		 * @brief Number of neighbours explored in the execution of
		 * the procedure.
		 */
		size_t n_neighbours_explored;
	
	public:
		/// Default constructor.
		problem();
		/// Destructor.
		virtual ~problem();
		
		// Constructing a solution
		
		/**
		 * @brief Constructs an empty solution.
		 * 
		 * Must copy, at least, the reference to the random number
		 * generator through the @ref copy() function.
		 */
		virtual problem *empty() const = 0;
		
		/**
		 * @brief Construct a solution from scratch.
		 * 
		 * May use any algorithm (for example, but not limited to,
		 * greedy algorithms, random algorithms, ...).
		 * 
		 * @returns Returns the evaluation of the solution.
		 */
		virtual double greedy_construct() throw(infeasible_exception) = 0;
		
		/**
		 * @brief Find the best neighbour of this problem.
		 * 
		 * Explores this solution's neighbourhood and stores:
		 * - the best neighbour if BI is true (best improvement)
		 * - the first best neighbour if BI is false (first improvement)
		 * 
		 * The best neighbour is the solution that maximises the
		 * @ref evaluate() function.
		 * 
		 * @param[out] best_neighbour The best neighbour of this problem.
		 * @param[in] p The local search policy.
		 * 
		 * @post Sets @ref n_neighbours_explored to the amounts of
		 * neighbours explored in this procedure.
		 */
		virtual void best_neighbour(pair<problem*, double>& best_neighbour, const local_search_policy& p = Best_Improvement) = 0;
		
		/**
		 * @brief Constructs a randomized solution using the restricted
		 * candidate list method.
		 * 
		 * The restricted candidate list built in the function must be
		 * built using the parameter @e alpha (a candidate should be
		 * in the list only if its cost satisfies.
		 *
		 \verbatim
			cost <= min_cost + alpha*(max_cost - min_cost)
		 \endverbatim
		 * 
		 * @param[in] rng The random number generator
		 * @param[in] alpha Parameter used to build the Restricted Candidate List.
		 * @returns Returns the evaluation of the solution.
		 */
		virtual double random_construct(random_generator *rng, double alpha) throw(infeasible_exception) = 0;
		
		/**
		 * @brief Constructs a solution from a given chromosome.
		 * @returns Returns the evaluation of the solution (its cost).
		 */
		virtual double decode(const chromosome& c) throw(infeasible_exception) = 0;
		
		// Evaluating a solution
		
		/**
		 * @brief Evaluates this instance of the problem.
		 * @returns Returns a floating point value representing its cost.
		 */
		virtual double evaluate() const = 0;
		
		// Debugging a solution
		
		/// @brief Writes into the output stream the instance of this problem.
		virtual void print(const string& tab = "", ostream& os = cout) const = 0;
		
		/**
		 * @brief Checks all constraints regarding solution feasibility.
		 * @returns Returns true if the solution is feasible. Returns false
		 * if otherwise.
		 */
		virtual bool sanity_check(const string& tab = "", ostream& os = cerr) const = 0;
		
		// Memory handling
		
		/**
		 * @brief Creates a new instance of the problem.
		 * 
		 * Returns a new object with the contents of this instance.
		 */
		virtual problem *clone() const = 0;
		
		/**
		 * @brief Creates a new instance of the problem.
		 * 
		 * Copies the contents of the instance @e p.
		 * 
		 * @param[in] p The object to be copied.
		 */
		virtual void copy(const problem *p) = 0;
		
		/**
		 * @brief Clears the memory used by the instance of this problem.
		 * 
		 * @post Everything is reset so that any of the constructive methods
		 * would create a solution to the problem if it were to be called.
		 * 
		 * The constructive methods are @ref greedy_construct() and
		 * @ref random_construct().
		 */
		virtual void clear() = 0;
		
		// Getters
		
		/**
		 * @brief Returns the number of neighbours that have been explored.
		 * 
		 * @returns When the
		 * @ref best_neighbour(pair<problem*, double>&, const local_search_policy&)
		 * function is called a number of neighbours are explored.
		 * 
		 * The variable @ref n_neighbours_explored is modified accordingly.
		 * 
		 * This function returns the value of @ref n_neighbours_explored.
		 */
		size_t get_n_neighbours_explored() const;
};

} // -- namespace structures
} // -- namespace metaheuristics
