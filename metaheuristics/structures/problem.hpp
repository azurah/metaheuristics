/*********************************************************************
 *
 * Collection of heuristic and metaheuristic algorithms
 *
 * Copyright (C) 2018-2020  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 *********************************************************************/

#pragma once

// C++ includes
#include <fstream>

// metaheuristics includes
#include <metaheuristics/structures/infeasible_exception.hpp>
#include <metaheuristics/structures/chromosome.hpp>
#include <metaheuristics/structures/policies.hpp>
#include <metaheuristics/random/random_generator.hpp>

namespace metaheuristics {
namespace structures {

/**
 * @brief Definition of the problem to be solved.
 * 
 * This interface defines several functions to handle a problem's
 * solution.
 * 
 * Consists of basic functions like copying, freeing, evaluating,
 * and simple construction of solutions. Also, it defines functions
 * for debugging, like sanity checks, printing, ...
 * 
 * @param G The type of engine used in the random generator in function
 * @ref random_construct(drandom_generator<G,size_t>*, double).
 */
template<
	class G = std::default_random_engine
>
class problem {
	public:
		/// Default constructor.
		problem() = default;
		/// Destructor.
		virtual ~problem() = default;
		
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
		virtual double greedy_construct() noexcept(false) = 0;
		
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
		virtual void best_neighbour(
			std::pair<problem*, double>& best_neighbour,
			const local_search_policy& p = Best_Improvement
		) = 0;
		
		/**
		 * @brief Constructs a randomized solution using the restricted
		 * candidate list method.
		 * 
		 * The restricted candidate list built in the function must be
		 * built using the parameter @e alpha (a candidate should be
		 * in the list only if its cost satisfies).
		 *
		 \verbatim
			cost <= min_cost + alpha*(max_cost - min_cost)
		 \endverbatim
		 * 
		 * @param[in] rng The random number generator
		 * @param[in] alpha Parameter used to build the Restricted Candidate List.
		 * @returns Returns the evaluation of the solution.
		 */
		virtual double random_construct(
			random::drandom_generator<G,size_t> *rng,
			double alpha
		)
		noexcept(false) = 0;
		
		/**
		 * @brief Constructs a solution from a given chromosome.
		 * @returns Returns the evaluation of the solution (its cost).
		 */
		virtual double decode(const chromosome& c) noexcept(false) = 0;
		
		// Evaluating a solution
		
		/**
		 * @brief Evaluates this instance of the problem.
		 * @returns Returns a floating point value representing its cost.
		 */
		virtual double evaluate() const = 0;
		
		// Debugging a solution
		
		/**
		 * @brief Writes into the output stream the instance of this problem.
		 * 
		 * Basically, enough information so as to understand the solution properly.
		 */
		virtual void print(std::ostream& os, const std::string& tab = "")
		const = 0;
		
		/**
		 * @brief Checks all constraints regarding solution feasibility.
		 * @returns Returns true if the solution is feasible. Returns false
		 * if otherwise.
		 */
		virtual bool sanity_check(std::ostream& os, const std::string& tab = "")
		const = 0;
		
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

	protected:
		/**
		 * @brief Number of neighbours explored in the execution of
		 * the procedure.
		 */
		size_t n_neighbours_explored = 0;
};

} // -- namespace structures
} // -- namespace metaheuristics

#include <metaheuristics/structures/problem.cpp>
