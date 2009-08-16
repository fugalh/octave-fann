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

DEFUN_DLD(__fann_train_on_file, args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} __fann_train_on_file(@var{ann}, @var{filename}, ...)\n\
@strong{This is an internal function of the Octave-FANN binding\n\
- Use with Caution}. This calls fann_train_on_file, see the FANN\n\
documentation. The function also accepts some optional parameters,\n\
see @code{fann_train} for details. Users should always use\n\
@code{fann_train} instead of this function.\n\n\
@seealso{fann_train}\n\
@end deftypefn\n\
")
{


    if (args.length() < 2 || args(0).type_name() != "fann" || !args(1).is_string()) {
        error("First argument must be a FANN and second must be a filename.\n");
        return octave_value();
    }
    
    /* Get the FANN and the filename.
     * Note: I can't seem to get this to work without a non-const filename. */
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& of = ((const oct_fann &)rep);
    string cpp_fn = args(1).string_value();
    char filename[cpp_fn.size() + 1];
    strcpy(filename, cpp_fn.c_str());
    
    /* Get the parameters or use defaults */
    unsigned int max_iterations = 500000;
    unsigned int iterations_between_reports = 1000;
    float desired_error = 0.0001;
    int i = 2;
    while (i < args.length()) {
        if (args(i).is_string()) {
            const string option = args(i).string_value();
            i += 1;
            if (option == "MaxIterations") {
                if (i >= args.length() || !args(i).is_real_scalar()) {
                    error("MaxIterations must be given as a positive integer.\n");
                    return octave_value();
                }
                max_iterations = args(i).int_value();
            } else if (option == "IterationsBetweenReports") {
                if (i >= args.length() || !args(i).is_real_scalar()) {
                    error("IterationsBetweenReports must be given as a positive integer.\n");
                    return octave_value();
                }
                iterations_between_reports = args(i).int_value();
            } else if (option == "DesiredError") {
                if (i >= args.length() || !args(i).is_real_scalar()) {
                    error("DesiredError must be given as a real scalar\n");
                    return octave_value();
                }
                desired_error = args(i).double_value();
            } else {
                const string uk_param = "Unknown parameter: ";
                const string error_message = uk_param + option;
                error(error_message.c_str());
                return octave_value();
            }
            i ++;
        } else {
            print_usage();
            return octave_value();
        }
    }

    /* Perform the training */
    fann_train_on_file(of.ann, filename, max_iterations, iterations_between_reports, desired_error);
    
    return octave_value();
}
