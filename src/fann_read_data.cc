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

DEFUN_DLD(fann_read_data,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} @var{data} = fann_read_data(@var{filename})\n\
Reads a data set from @var{filename}. The file should have the\n\
following format:\n\
@example\n\
num_train_data num_input num_output\n\
inputdata seperated by space\n\
outputdata seperated by space\n\
\n\
.\n\
.\n\
.\n\
\n\
inputdata seperated by space\n\
outputdata seperated by space\n\
@end example\n\n\
The output @var{data} will be a structure with the following keys:\n\
@table @code\n\
@item input\n\
A @var{N}x@var{I} matrix, where @var{N} is the number of data\n\
elements, and @var{I} is the number of inputs.\n\
@item target\n\
A @var{N}x@var{O} matrix, where @var{N} is the same as above, and\n\
@var{O} is the number of outputs.\n\
@end table\n\
@end deftypefn\n\
")
{
    if (args.length() != 1) {
        print_usage();
        return octave_value();
    } 
    
    if (args(0).is_string()) {
        // Get the filename.
        string filename = args(0).string_value();
        char c_filename[filename.length()+1];
        strcpy(c_filename, filename.c_str());
        struct fann_train_data *data = fann_read_train_from_file(c_filename);
        if (data != NULL && data->errno_f == (unsigned int)FANN_E_NO_ERROR) {
            Octave_map *oct_data = data2struct(data);
            fann_destroy_train(data);
            return octave_value(*oct_data);
        } else {
            error("Couldn't read data.\n");
            return octave_value();
        }
    } else {
        error("Argument must be a filename.\n");
        return octave_value();
    }
    
}

