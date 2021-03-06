#pragma once

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
using namespace std;

// metaheuristics includes
#include <metaheuristics/structures.hpp>
using namespace metaheuristics;
using namespace structures;

// Custom includes
#include "centre_type.hpp"
#include "city.hpp"

class interface : public problem<> {
	public:
		enum role {
			none = 0,
			primary = 1,
			secondary = 2
		};
		
	public:
		interface() = default;
		virtual ~interface() = default;
		
		void read_from_file(istream& fin = cin);
		void print_input_data(ostream& fout = cout);
		
		/* GETTERS */
		
		int get_n_cities() const;
		int get_n_locations() const;
		int get_n_centre_types() const;
		
		// ----------------------------
		// -- PROBLEM IMPLEMENTATION --
		// ----------------------------
		
		/* Evaluating a solution */
		
		// Evaluates the instance of this problem returning a scalar value
		// representing its cost.
		double evaluate() const;
		
		/* Debugging a solution */
		
		// Writes into the output stream the instance of this problem.
		void print(ostream& os, const string& tab = "") const;
		
		// Checks all constraints regarding solution feasibility. Returns
		// true if the solution is feasible. Returns false otherwise.
		bool sanity_check(ostream& os, const string& tab = "") const;
		
		/// Memory handling
		
		// Clears the memory used by the instance of this problem.
		// Everything is reset so that the method construct would create
		// a solution to the problem if it were to be called.
		void clear();
		
	protected:

		double D;

		int n_locations;
		int n_centres;
		int n_cities;

		vector<point> locations;
		vector<city> cities;
		vector<centre_type> centres;

		vector<int> sorted_cities;
		vector<int> sorted_centres;

		/* Instance solution */

		// Capacity occupied for every location
		// cap_location[i] = s -> location 'i', serving some cities with
		//     a primary role and some others with a secondary role, is
		//     occupied by 's', the sum of the population of the cities
		//     served with a primary role plus 10% of the sum of the
		//     population of the cities served with a secondary role.
		vector<double> cap_location;

		// Assignation of a center type to a location
		// location_centre_type[i] = j -> center type 'j' is assigned to location 'i'
		vector<int> location_centre_type;

		// Assignation of a location to a city
		// cit_by_prim[c] = l -> city 'c' is served by location 'l' with primary role
		// cit_by_sec[c] = l  -> city 'c' is served by location 'l' with secondary role
		vector<int> cit_by_prim;
		vector<int> cit_by_sec;

		/* Miscellaneous functions */

		// Copies all input data and solution data
		void copy_interface(const interface *i);

		// Copies only input data
		void copy_empty(const interface *i);

		// Erases solution data
		void empty_interface();

		/* Constraint checking */

		// Check that there is a distance of at least D to the rest of
		// locations with a centre installed.
		bool separated_by_D(int loc_idx) const;

		// Check that there is a distance of at least D to the locations
		// in 'locs'
		bool separated_by_D(int loc_idx, const set<int>& locs) const;

		// Assuming that the location is assigned to serve some cities,
		// check that the working distance would be exceeded in case
		// 'centre_idx' was to be installed in that location.
		// Returns true if exceeded.
		bool working_distance_exceeded(int loc_idx, int centre_idx) const;

		// Assuming that the location is assigned to serve some cities,
		// check whether the capacity would be exceeded in case 'centre_idx'
		// was to be installed in that location.
		// Returns true if exceeded.
		bool capacity_exceeded(int loc_idx, int centre_idx) const;

		// Assuming that the location is assigned to serve some cities,
		// check that the working distance and capacity constraints
		// would be satisfied in case 'centre_idx' was to be installed
		// in that location.
		// Returns true if satisfied.
		bool joined_constraints_satisfied(int loc_idx, int centre_idx) const;

		// Sets to 'orimary' the positions 'c' in 'cities_served' such that
		//     cit_by_prim[c] == loc_idx or
		// and to 'secondary' the positions 'c' in 'cities_served' such that
		//     cit_by_sec[c] == loc_idx
		// and sets to 'none' the rest and stores in 'how_many' the amounts
		// of positions set to 'primary' or 'secondary'.
		void cities_served_by_location(int loc_idx, vector<role>& cities_served, int& how_many) const;

		/* Utils */

		// Returns the distance between the city 'city_idx' and the
		// location 'loc_idx'
		double dist_city_loc(int city_idx, int loc_idx) const;
};

