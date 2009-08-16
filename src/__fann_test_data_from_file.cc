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

DEFUN_DLD(__fann_test_data_from_file, args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} @var{mse} = __fann_test_data_from_file(@var{ann}, @var{filename})\n\
@strong{This is an internal function of the Octave-FANN binding\n\
- Use with Caution}. This calls fann_test_data, see the FANN\n\
documentation. Users should always use @code{fann_test}\n\
instead of this function.\n\n\
@seealso{fann_test}\n\
@end deftypefn\n\
")
{
    if (args.length() < 2 || args(0).type_name() != "fann" || !args(1).is_string()) {
        error("First argument must be a FANN and second argument must be a filename\n");
        return octave_value();
    }
    
    /* Get the FANN and the filename. */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);
    const string filename = args(1).string_value();
    
    /* Get the data */
    char c_filename[filename.size()+1];
    strcpy(c_filename, filename.c_str());
    struct fann_train_data *data = fann_read_train_from_file(c_filename);

    if (data == NULL || data->errno_f != (unsigned int)FANN_E_NO_ERROR) {
        error("Error reading the data from file.\n");
        return octave_value();
    }

    /* Perform the test */
    const double MSE = fann_test_data(ann.ann, data);
    
    /* Destroy the data */
    fann_destroy_train(data);
    
    return octave_value(MSE);
}

