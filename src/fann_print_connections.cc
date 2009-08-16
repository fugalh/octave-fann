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

DEFUN_DLD(fann_print_connections, args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} fann_print_connections(@var{ann})\n\
fann_print_connections will print the connections of the FANN\n\
neural network @var{ann} in a compact matrix, for easy viewing\n\
of the internals of the ann.\n\
\n\
@example\n\
The output from fann_print_connections on a small (2 2 1) network\n\
trained on the xor problem:\n\
\n\
Layer / Neuron 012345\n\
L   1 / N    3 ddb...\n\
L   1 / N    4 bbb...\n\
L   2 / N    6 ...cda\n\
\n\
This network have five real neurons and two bias neurons. This gives\n\
a total of seven neurons named from 0 to 6. The connections between\n\
these neurons can be seen in the matrix. \".\" is a place where there\n\
is no connection, while a character tells how strong the connection\n\
is on a scale from a-z. The two real neurons in the hidden layer\n\
(neuron 3 and 4 in layer 1) has connection from the three neurons in\n\
the previous layer as is visible in the first two lines.\n\
The output neuron (6) has connections form the three neurons in the\n\
hidden layer 3 - 5 as is visible in the last line.\n\
@end example\n\
\n\
To simplify the matrix output neurons is not visible as neurons that\n\
connections can come from, and input and bias neurons are not visible\n\
as neurons that connections can go to.\n\n\
@seealso{fann_show}\n\
@end deftypefn\n\
")
{

    if (args.length() != 1 || args(0).type_name() != "fann" ) {
        error("First argument must be a FANN.\n");
        return octave_value();
    }
    
    /* Get the FANN */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);
    
    /* Call FANN */
    fann_print_connections(ann.ann);

    return octave_value();
}

