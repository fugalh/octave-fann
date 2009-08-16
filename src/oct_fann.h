/*
Copyright (C) 2005 Søren Hauberg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

Author: Søren Hauberg <hauberg at gmail dot com>

2005-06-02 Søren Hauberg <hauberg at gmail dot com>
* Initial revision

*/

#include <iostream>
#include <octave/oct.h>
#include <octave/parse.h>
#include <octave/dynamic-ld.h>
#include <octave/oct-map.h>
#include <octave/oct-stream.h>
#include <octave/ov-base-scalar.h>
#include <vector>
#include <string>
#include "fann.h"
#include "fann_error.h"

#ifndef OCT_FANN_H
#define OCT_FANN_H

using namespace std;

/* Some simple macros for iteration */
#define foreach_layer(LAYER, NET) \
        for (struct fann_layer *(LAYER) = (NET)->first_layer; (LAYER) < (NET)->last_layer; (LAYER)++)
#define foreach_neuron(NEURON, LAYER) \
        for (struct fann_neuron *(NEURON) = (LAYER)->first_neuron; (NEURON) < (LAYER)->last_neuron; (NEURON)++)


class oct_fann: public octave_base_value 
{
	public: 

	// Constructor
	oct_fann() : octave_base_value()
	{
		ann = NULL;
	}

	oct_fann(const string &filename) {
		char c_filename[filename.size() + 1];
		strcpy(c_filename, filename.c_str());
		ann = fann_create_from_file(c_filename);
	}

	oct_fann(unsigned int num_layers, unsigned int *layers, 
			 float connection_rate, float learning_rate, const string &type) : octave_base_value()
	{
		if (type == "feedforward") {
			ann = fann_create_sparse_array(connection_rate, 
			                        num_layers, layers);
		} else if (type == "shortcut") {
			ann = fann_create_shortcut_array(num_layers, layers);
		} else {
			error("Internal error in the FANN binding: Wrong type in network constructor.\n");
		}
	}

	// Destructor
	~oct_fann(void) {
		if (ann != NULL) {
			fann_destroy(ann);
		}
	}
	
	void print (std::ostream& os, bool pr_as_read_syntax = false) const {
		os << "Octave FANN object." << endl;
	}

	void save_fann(const string &filename) const {
		char c_filename[filename.size() + 1];
		strcpy(c_filename, filename.c_str());
		fann_save(ann, c_filename);
	}
	
	int save_fixed_fann(const string &filename) const {
		char c_filename[filename.size() + 1];
		strcpy(c_filename, filename.c_str());
		return fann_save_to_fixed(ann, c_filename);
	}

	void load_fann(const string &filename) {
		if (ann != NULL) {
			fann_destroy(ann);
		}
		char c_filename[filename.size() + 1];
		strcpy(c_filename, filename.c_str());
		ann = fann_create_from_file(c_filename);
	}

	struct fann * get_fann() { return ann; }

	bool is_constant (void) const { return true; }
	bool is_defined (void) const { return true; }

	struct fann *ann;
	private:

	DECLARE_OCTAVE_ALLOCATOR
	DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA
};

DEFINE_OCTAVE_ALLOCATOR (oct_fann);
DEFINE_OV_TYPEID_FUNCTIONS_AND_DATA (oct_fann, "fann", "fann");

/* Some auxilary functions */
void lowercase(string &str) {
	for (unsigned int i=0; i < str.size(); i++) {
		str[i] = tolower(str[i]);
	}
}

Octave_map* data2struct(struct fann_train_data *data) {
    Octave_map *out_map = new Octave_map;
    const int num_data  = data->num_data;
    const int num_input = data->num_input;
    const int num_output= data->num_output;
    
    /* Input */
    Matrix input(num_data, num_input);
    for (int r=0; r < num_data; r++) {
        for (int c=0; c < num_input; c++) {
            input(r,c) = *(*data->input + r*num_input + c);
        }
    }

    /* Output */
    Matrix output(data->num_data, data->num_output);
    for (int r=0; r < num_data; r++) {
        for (int c=0; c < num_output; c++) {
            output(r,c) = *(*data->output + r*num_output + c);
        }
    }

    out_map->assign("input",  octave_value(input));
    out_map->assign("target", octave_value(output));

    return out_map;
}

/*
struct fann_train_data* struct2data(Octave_map &map) {
    if (!map.contains("input") || !map.contains("target")) {
        error("Training data must contain both 'input' and 'target'\n");
        return NULL;
    }
    const Cell input  = map.contents("input");
    const Cell target = map.contents("target");
    
}
*/
#endif
