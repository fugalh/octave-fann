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

DEFUN_DLD(fann_show,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} fann_show(@var{ann})\n\
Shows the FANN neural network. Blue connections are used when the\n\
weight is positive and red is used otherwise. The upper neuron in\n\
each layer is allways the bias neuron.\n\
This is mostly useful on smaller networks, as the function is\n\
rather slow, and the plot becomes hard to read on large networks.\n\
In such situations @code{fann_print_connections} is usualy better.\n\n\
@seealso{fann_print_connections}\n\
@end deftypefn\n\
")
{
    if (args.length() != 1 || args(0).type_name() != "fann") {
        error("First argument must be a FANN.\n");
        return octave_value();
    }
    
    /* Get the FANN and the filename. */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);
    
    feval("hold", octave_value("on"));
    
    map<struct fann_neuron*, double> x_map;
    map<struct fann_neuron*, double> y_map;
    ColumnVector x_vec(2);
    ColumnVector y_vec(2);
    double x, y, y_max;
    x = y_max = 1;
    /* For each layer: */
    struct fann_layer *prev_layer = NULL;
    foreach_layer(layer, ann.ann) {
        /* For each neuron: */
        const int num_neurons = layer->last_neuron - layer->first_neuron;
        y = 1;
        foreach_neuron(neuron, layer) {
            x_map[neuron] = x;
            y_map[neuron] = y - (num_neurons+1)/2.0;
            /* For each connection: (bias connections excluded) */
            for (unsigned int c=0; c < neuron->num_connections; c++) {
                const fann_type weight = neuron->weights[c];
                struct fann_neuron *neighbour = neuron->connected_neurons[c];
                octave_value_list plot_args;
                x_vec(0) = x_map[neuron];
                x_vec(1) = x_map[neighbour];
                y_vec(0) = y_map[neuron];
                y_vec(1) = y_map[neighbour];
                plot_args.append(x_vec);
                plot_args.append(y_vec);
                if (weight > 0) {
                    plot_args.append(octave_value("-b"));
                } else {
                    plot_args.append(octave_value("-r"));
                }
                feval("plot", plot_args);
            }
            y++;
        }
        /* Now we plot bias connections */
        if (prev_layer != NULL) {
            const int pre_num_neurons = prev_layer->last_neuron - prev_layer->first_neuron;
            
        }
        
        y_max = max(y, y_max);
        x ++;
        prev_layer = layer;
    }
    
    /* Find coordinates of the neurons */
    ColumnVector x_vector(x_map.size());
    ColumnVector y_vector(y_map.size());
    map<struct fann_neuron*, double>::iterator iter;
    unsigned int i;
    for(iter = x_map.begin(), i = 0; iter != x_map.end(); iter++, i++) {
        x_vector(i) = iter->second;
    }    
    for(iter = y_map.begin(), i = 0; iter != y_map.end(); iter++, i++) {
        y_vector(i) = iter->second;
    }
    
    /* Set the range */
    char tmp[50];
    sprintf(tmp, "xrange [0.7 to %.1f]", x-0.7);
    octave_value gnuplot_args(tmp);
    feval("__gnuplot_set__", gnuplot_args);
    sprintf(tmp, "yrange [-%.1f to %.1f]", y_max/2.0, y_max/2.0);
    gnuplot_args = octave_value(tmp);
    feval("__gnuplot_set__", gnuplot_args);
    
    /* Do the plotting */
    gnuplot_args = octave_value("pointsize 3");
    feval("__gnuplot_set__", gnuplot_args);
    octave_value_list plot_args;
    plot_args.append(octave_value(x_vector));
    plot_args.append(octave_value(y_vector));
    plot_args.append(octave_value("@*k"));
    feval("plot", plot_args);
    feval("axis", octave_value("off"));
    // Clean up plotting
    feval("hold", octave_value("off"));
    gnuplot_args = octave_value("pointsize");
    feval("__gnuplot_set__", gnuplot_args);
    gnuplot_args = octave_value("xrange restore");
    feval("__gnuplot_set__", gnuplot_args);
    gnuplot_args = octave_value("yrange restore");
    feval("__gnuplot_set__", gnuplot_args);
    
    return octave_value();
}

