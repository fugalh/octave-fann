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

#include "oct_fann.h"

DEFUN_DLD(fann_create,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} @var{ann} = fann_create(@var{layer_sizes}, \"Option1\", @var{value1}, ...)\n\
Creates FANN neural network with length(@var{layer_sizes}) layers\n\
(including input and output layers). The first layer (i.e. the input\n\
layer) will have @var{layer_sizes}(1) neurons, and so on.\n\
\n\
The Option parameters may be any of the following:\n\
@table @code\n\
@item \"ConnectionRate\"\n\
The corresponding @var{value} parameter will set the connection\n\
rate of the network, i.e. the fraction of connections between the\n\
neurons of the network. The default value is 1.0, that is a fully\n\
connected network.\n\
@item \"LearningRate\"\n\
The corresponding @var{value} sets the learning rate of the\n\
training algorithm used to train this network. The default value\n\
is 0.7.\n\
@item \"Feedforward\"\n\
Creates a standard feedforward network, where neurons in layer\n\
@var{i} only are connected to neurons in layer @var{i}+1.\n\
There is no corresponding @var{value} parameter.\n\
This is the default setting.\n\
@item \"Shortcut\"\n\
Creates a shortcut network, i.e. a network where neurons in layer\n\
@var{i} are connected to neurons in layer @var{j}, for all\n\
@var{j} > @var{i}.\n\
There is no corresponding @var{value} parameter.\n\
@end table\n\
\n\
Example:\n\
@example\n\
To create a standard feedforward network with two input\n\
neurons, one output neuron, and one hidden layer with one neuron:\n\
@code{@var{ann} = fann_create([2 1 1]);}\n\
\n\
If you want the same network but with a lower connection rate,\n\
i.e. 0.5:\n\
@code{@var{ann} = fann_create([2 1 1], \"ConnectionRate\", 0.5);}\n\
@end example\n\
@deftypefnx{Function File} @var{ann} = fann_create(@var{filename})\n\
Loads a FANN network from the file @var{filename}.\n\
The file must contain a valid FANN network, such as one created by\n\
@code{fann_save}.\n\n\
@seealso{fann_train, fann_save, and fann_run}\n\
@end deftypefn\n\
")
{
    if (args.length() < 1) {
        print_usage();
        return octave_value();
    } 
    
    /* If the first argument is a filename, we read the network.
     * Else if the first argument is a matrix we use it to determine
     * the size of the new network. */
    Matrix layers;
    if (args(0).is_string()) {
        // Get the filename.
        string filename = args(0).string_value();

        // Create the network
        oct_fann *ann = new oct_fann(filename);
        if (ann->ann == NULL) {
            delete ann;
            error("Unable to create network.\n");
            error("Note: fixed point networks cannot be loaded into Octave.\n");
            return octave_value();
        }
        return octave_value(ann);
    
    } else if (args(0).is_real_matrix()) {
        // Get the size of the layers.
        layers = args(0).matrix_value();
    } else {
        error("First argument must be a scalar vector or a filename\n");
        return octave_value();
    }
    
    const unsigned int num_layers = layers.length();
    unsigned int s[num_layers];
    for (unsigned int i = 0; i < num_layers; i++) {
        s[i] = (unsigned int)layers(i);
    }

    /* Get the parameters or use defaults */
    float learning_rate = 0.7;
    float connection_rate = 1;
    const string feedforward("feedforward");
    const string shortcut("shortcut");
    string type = feedforward;
    int i = 1;
    while (i < args.length()) {
        if (args(i).is_string()) {
            string option(args(i).string_value());
            lowercase(option);
            if (option == "learningrate") {
                i += 1;
                if (i >= args.length() || !args(i).is_real_scalar()) {
                    error("LearningRate must be given as a real scalar\n");
                    return octave_value();
                }
                learning_rate = args(i).double_value();
          
            } else if (option == "connectionrate") {
                i += 1;
                if (i >= args.length() || !args(i).is_real_scalar()) {
                    error("ConnectionRate must be given as a real scalar\n");
                    return octave_value();
                }
                connection_rate = args(i).double_value();
                
            } else if (option == feedforward || option == shortcut) {
                type = option;
                
            } else {
                error("Unknown parameter: %s\n", option.c_str());
                return octave_value();
            }
            i ++;
            
        } else {
            print_usage();
            return octave_value();
        }
    }

    /* Create the network */
    oct_fann *nn = new oct_fann(num_layers, s, connection_rate, learning_rate, type);
    return octave_value(nn);
}

