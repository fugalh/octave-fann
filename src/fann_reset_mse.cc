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

DEFUN_DLD(fann_reset_mse,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} fann_reset_mse(@var{ann})\n\
Resets the mean square error from the network.\n\n\
@seealso{fann_get_mse, and fann_train}\n\
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

    /* Reset MSE */
    fann_reset_MSE(ann.ann);
    
    return octave_value();
}

