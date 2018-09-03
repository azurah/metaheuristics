#pragma once

// C++ includes
#include <iostream>
#include <iomanip>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/algorithms/problem.hpp>
#include <metaheuristics/misc/timing.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace mtime;

/**
 * @brief Local Search heuristic algorithm.
 * 
 * Given a maximum number of iterations and a local search policy
 * (see @ref metaheuristics::structures::local_search_policy) apply
 * the local search algorithm on an instance of a problem.
 * 
 * See @ref execute_algorithm for details on how this
 * algorithm works.
 * 
 * Compile the library with the flag
	\verbatim
	LOCAL_SEARCH_VERBOSE
	\endverbatim
 * to see the progress of the algorithm.
 * 
 * In addition to the previous flag, one can also compile the library
 * with the flag
	\verbatim
	LOCAL_SEARCH_DEBUG
	\endverbatim
 * to run the sanity check on every "best neighbour" found.
 */
class local_search : public metaheuristic {
	private:
		/// Total execution time of the algorithm.
		double total_time;
		/// Total execution time to explore the different neighbourhoods.
		double neighbourhood_time;
		
		/// Maximum number of iterations.
		size_t MAX_ITER;
		/// Current iteration of the algorithm.
		size_t ITERATION;
		/// Policy of the local search.
		local_search_policy LSP;
				
	public:
		/// Default constructor.
		local_search();
		/// 
		/**
		 * @brief Constructor with maximum number of iterations and search policy.
		 * @param m Maximum number of iterations of the algorithm.
		 * @param lsp Local Search policy (see
		 * @ref metaheuristics::structures::local_search_policy).
		 */
		local_search(size_t m, const local_search_policy& lsp);
		/// Destructor
		~local_search();
		
		// SETTERS
		
		/**
		 * @brief Sets the local search policy.
		 * 
		 * Sets the value of @ref LSP to @e lsp.
		 */
		void set_local_search_policy(const local_search_policy& lsp);
		/**
		 * @brief Sets the maximum number of iterations of the algorithm.
		 * 
		 * Sets the value of @ref MAX_ITER to @e m.
		 */
		void set_max_iterations(size_t m);
		/**
		 * @brief Resets the algorithm to its initial state.
		 * 
		 * Sets to 0 the internal variables @ref total_time, @ref neighbourhood_time,
		 * @ref MAX_ITER, @ref ITERATION. The policy @ref LSP is not
		 * modified.
		 */
		void reset_algorithm();
		
		// GETTERS
		
		/**
		 * @brief Returns the total execution time of the algorithm.
		 * @returns Returns the value that @ref total_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_total_time() const;
		/**
		 * @brief Returns the execution time needed to explore the neighbourhood.
		 * @returns Returns the value that @ref neighbourhood_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_neighbourhood_time() const;
		/**
		 * @brief Returns the execution time needed to explore the neighbourhood.
		 * @returns Returns the value the variable @ref ITERATION has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		size_t get_n_iterations() const;
		/**
		 * @brief Returns the policy of the search.
		 * @returns Returns the value of @ref LSP.
		 */
		local_search_policy get_local_search_policy() const;
		/**
		 * @brief Returns the number of iterations.
		 * @returns Returns the value of @ref MAX_ITER.
		 */
		size_t get_max_iterations() const;
		
		/**
		 * @brief Execute the local search algorithm.
		 * 
		 * Given an instance of a problem @e p, with cost @e c,
		 * this algorithm works as follows: 
		 * - Define BEST = @e p
		 * - Apply while there is improvement and for a maximum number
		 * 		of iterations:
		 * 		- Find the best solution among those in its neighbourhood,
		 * 		according to the search policy.
		 * 		- Define BEST to be that solution
		 * - Return BEST
		 * 
		 * The number of iterations can be set in the constructor
		 * (see @ref local_search(size_t, const local_search_policy&))
		 * or in the method @ref set_max_iterations(size_t).
		 * 
		 * @param[in] p The instance of the problem.
		 * @param[in] c The cost of the instance p at the beginning of
		 * the execution.
		 * @param[out] p Whence the algorithm has finished p contains
		 * an improved solution to the problem.
		 * @pre The method @ref reset_algorithm does not need to be called.
		 */
		bool execute_algorithm(problem *p, double& c);
		
		/**
		 * @brief Prints a summary of the performance of the algorithm.
		 * 
		 * Prints to standard output a message with the following format:
		\verbatim
		Local Search algorithm performance
		    Number of iterations:                   INT
		    Total execution time:                   DOUBLE s
		    Average iteration time:                 DOUBLE s
		    Total neihgbourhood exploration time:   DOUBLE s
		    Average neihgbourhood exploration time: DOUBLE s
		\endverbatim
		 */
		void print_performance() const;
};

} // -- namespace algorithms
} // -- namespace metaheuristics
