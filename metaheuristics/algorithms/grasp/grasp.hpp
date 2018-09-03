#pragma once

// C++ includes
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/algorithms/local_search/local_search.hpp>
#include <metaheuristics/algorithms/problem.hpp>

namespace metaheuristics {
namespace algorithms {

/**
 * @brief GRASP metaheuristic algorithm.
 * 
 * Implementation of the Greedy Randomised Adaptative-Search Procedure.
 * 
 * See @ref execute_algorithm for details on how this
 * algorithm works.
 * 
 * Compile the library with the flag
	\verbatim
	GRASP_VERBOSE
	\endverbatim
 * to see the progress of the algorithm.
 * 
 * In addition to the previous flag, one can also compile the library
 * with the flag
	\verbatim
	GRASP_DEBUG
	\endverbatim
 * to run the sanity check on every solution constructed randomly,
 * and on every solution found by the local search procedure.
 * 
 * The local search procedure also has its own compilation flags.
 * See @ref local_search for details.
 */
class grasp : public metaheuristic {
	private:
		/// Total execution time of the algorithm.
		double total_time;
		/// Total execution time spent in constructing the initial
		/// randomised solution.
		double construct_time;
		/// Total execution time spent in the Local Search.
		double local_search_time;
		
		/// Maximum number of iterations of the GRASP algorithm.
		size_t MAX_ITER_GRASP;
		/// Maximum number of iterations of the Local Search procedure.
		size_t MAX_ITER_LOCAL;
		/// Parameter used to construct the Restricted Candidate List.
		double alpha;
		/// Policy for the Local Search procedure.
		local_search_policy LSP;
		
	public:
		/// Default constructor.
		grasp();
		/// Constructor with several parameters.
		grasp
		(
			size_t m_GRASP, size_t m_LOCAL, double a,
			const local_search_policy& lsp,
			random_number_generator *rng
		);
		/// Destructor.
		~grasp();
		
		// SETTERS
		
		/**
		 * @brief Sets the policy for the Local Search procedure.
		 * 
		 * Sets the value of @ref LSP to @e lsp.
		 */
		void set_local_search_policy(const local_search_policy& lsp); 
		/**
		 * @brief Sets the maximum number of iterations of the GRASP algorithm.
		 * 
		 * Sets the value of @ref MAX_ITER_GRASP to @e max.
		 */
		void set_max_iterations_grasp(size_t max);
		/**
		 * @brief Sets the maximum number of iterations of the Local Search procedure.
		 * 
		 * Sets the value of @ref MAX_ITER_LOCAL to @e max.
		 */
		void set_max_iterations_local(size_t max);
		/**
		 * @brief Resets the algorithm to its initial state.
		 * 
		 * Sets to 0 the internal variables @ref total_time, @ref construct_time,
		 * @ref local_search_time.
		 * 
		 * The other variables (@ref MAX_ITER_GRASP,
		 * @ref MAX_ITER_LOCAL, @ref alpha, @ref LSP) are not modified.
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
		 * @brief Returns the total time needed for the construction of random solutions.
		 * @returns Returns the value that @ref construct_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_construct_time() const;
		/**
		 * @brief Returns the total time spent in the Local Search procedure.
		 * @returns Returns the value that @ref local_search_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_local_search_time() const;
		
		/**
		 * @brief Returns the policy of the search.
		 * @returns Returns the value of @ref LSP.
		 */
		local_search_policy get_local_search_policy() const;
		/**
		 * @brief Returns the maximum number of iterations of the GRASP algorithm.
		 * @returns Returns the value of @ref MAX_ITER_GRASP.
		 */
		size_t get_max_iterations_grasp() const;
		/**
		 * @brief Returns the maximum number of iterations of the Local Search procedure.
		 * @returns Returns the value of @ref MAX_ITER_LOCAL.
		 */
		size_t get_max_iterations_local() const;
		
		/**
		 * @brief Execute the GRASP algorithm.
		 * 
		 * GRASP stands for Greedy Randomised Adaptative-Search Procedure.
		 * 
		 * Given an instance of a problem @e p, with cost @e c,
		 * this algorithm works as follows: 
		 * - Define BEST an empty solution
		 * - For as many iterations as @ref MAX_ITER_GRASP
		 * - Apply while there is improvement and for a maximum number
		 * 		of iterations:
		 * 		- Construct a random solution with a Restricted
		 * 		candidate list built using parameter @ref alpha.
		 * 		Define R as the result.
		 * 		- Apply the local search procedure on R for at most
		 * 		@ref MAX_ITER_LOCAL with policy @ref LSP. Define L as
		 * 		the result.
		 * 		- Keep the best solution between BEST and L.
		 * - Return BEST
		 * 
		 * The number of iterations can be set in the constructor
		 * (see @ref grasp(size_t,size_t,double,const local_search_policy&,random_number_generator*))
		 * or in the methods @ref set_max_iterations_grasp(size_t),
		 * @ref set_max_iterations_local(size_t).
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
		GRASP metaheuristic performance (for a total of INT iterations):
			Total execution time:      DOUBLE s
			Average iteration time:    DOUBLE s
			Total construction time:   DOUBLE s
			Average construction time: DOUBLE s
			Total local search time:   DOUBLE s
			Average local search time: DOUBLE s
		\endverbatim
		 */
		void print_performance() const;
};

} // -- namespace algorithms
} // -- namespace metaheuristics
