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

DEFUN_DLD(__fann_init_weights_on_file, args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} __fann_init_weights_on_file(@var{ann}, @var{filename})\n\
@strong{This is an internal function of the Octave-FANN binding\n\
- Use with Caution}. This calls fann_init_weights, see the FANN\n\
documentation. Users should always use @code{fann_reinit_weights}\n\
instead of this function.\n\n\
@seealso{fann_reinit_weights}\n\
@end deftypefn\n\
")
{

    if (args.length() != 2 || args(0).type_name() != "fann" || !args(1).is_string()) {
        error("First argument must be a FANN and second must be a filename.\n");
        return octave_value();
    }
    
    /* Get the FANN and the filename.
     * Note: I can't seem to get this to work without a non-const filename. */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& ann = ((const oct_fann &)rep);
    string filename = args(1).string_value();
    char c_filename[filename.size() + 1];
    strcpy(c_filename, filename.c_str());
    
    /* Get the data */
    struct fann_train_data *data = fann_read_train_from_file(c_filename);

    if (data == NULL || data->errno_f != (unsigned int)FANN_E_NO_ERROR) {
        error("Error reading the data from file.\n");
        return octave_value();
    }

    /* Reinit the weights */
    fann_init_weights(ann.ann, data);
    
    return octave_value();
}
