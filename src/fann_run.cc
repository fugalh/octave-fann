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

DEFUN_DLD(fann_run,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} fann_run(@var{ann}, @var{input})\n\
fann_run will run the matrix @var{input} through @var{ann},\n\
returning a matrix of outputs. Each row of @var{input} is treated as\n\
one input vector, so if the network has I input neurons @var{input}\n\
sjould be of size NxI and the output will be of size NxO, where O is\n\
the number of output neurons.\n\n\
@seealso{fann_create, fann_train, and fann_test}\n\
@end deftypefn\n\
")
{
    if (args.length() != 2 || args(0).type_name() != "fann" || !args(1).is_real_type()) {
        error("First argument must be a FANN and second argument must be an input matrix.\n");
        return octave_value();
    }
    
    /* Get the FANN and the filename. */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);
    const Matrix data = args(1).matrix_value();
    
    /* Make sure the input has the right size */
    if (data.columns() != fann_get_num_input(ann.ann)) {
        error("Wrong input size.\n");
        return octave_value();
    }
    
    const int num_data    = data.rows();
    const int input_size  = data.columns();
    const int output_size = fann_get_num_output(ann.ann);
    
    /* Create the output matrix */
    Matrix out_matrix(num_data, output_size);
    
    /* For each row in data: */
    for (int r=0; r < num_data; r++) {
        /* Copy the data to a fann_type array */
        fann_type input[input_size];
        for (int c = 0; c < input_size; c++) {
            input[c] = data(r,c);
        }
    
        /* Call fann_run */
        fann_type *out = fann_run(ann.ann, input);

        /* Copy the output to the output matrix */
        for (int c=0; c < output_size; c++) {
            out_matrix(r,c) = out[c];
        }
    }
    
    return octave_value(out_matrix);
}

