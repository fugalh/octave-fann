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

DEFUN_DLD(fann_randomize_weights,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} fann_randomize_weights(@var{ann}, @var{filename})\n\
@strong{This is an internal function of the Octave-FANN binding\n\
- Use with Caution}. This calls fann_randomize_weights, see the FANN\n\
documentation. Users should allways use @code{fann_reinit_weights}\n\
instead of this function.\n\n\
@seealso{fann_reinit_weights}\n\
@end deftypefn\n\
")
{

    if (args.length() != 1 && args.length() != 2) {
        print_usage();
        return octave_value();
    }
    
    if (args(0).type_name() != "fann" ) {
        error("First argument must be a FANN.\n");
        return octave_value();
    }
    
    /* Get the FANN */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);
    
    /* Get min and max weights or use defaults */
    fann_type min_weight = -1;
    fann_type max_weight =  1;
    if (args.length() == 2) {
        if (args(1).is_real_matrix()) {
            Matrix min_max = args(1).matrix_value();
            if (min_max.length() != 2) {
                error("Second argument must have length 2.\n");
                return octave_value();
            }
            min_weight = min_max(0);;
            max_weight = min_max(1);
        } else {
            error("MIN and MAX must be real scalars.\n");
            return octave_value();
        }
    }

    fann_randomize_weights(ann.ann, min_weight, max_weight);
    return octave_value();
}

