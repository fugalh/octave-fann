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

DEFUN_DLD(fann_size,args, nargout,
"-*- texinfo -*-\n\
@deftypefn {Function File} @var{s} = fann_size(@var{ann})\n\
Returns the size of the layers of @var{ann} excluding the bias\n\
neurons.\n\
@deftypefnx {Function File} [@var{s} @var{c}] = fann_size(@var{ann})\n\
Same as above except the number of connections is also returned.\n\
@end deftypefn\n\
")
{
    if (args.length() != 1 || args(0).type_name() != "fann") {
        error("First argument must be a FANN.\n");
        return octave_value();
    }

    /* Get the FANN */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);

    RowVector size;
    /* For each layer: */
    for (struct fann_layer *layer = ann.ann->first_layer; layer < ann.ann->last_layer; layer++) {
        /* For each neuron: */
        // The minus one comes from the bias neuron
        const int num_neurons = layer->last_neuron - layer->first_neuron - 1;
        size.resize_fill(1, size.length()+1, num_neurons);
    }

    octave_value_list output;
    output.append(octave_value(size));
    if (nargout > 1) {
        int connections = fann_get_total_connections(ann.ann);
        output.append(octave_value(connections));
    }

    return output;
}

