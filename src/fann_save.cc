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

DEFUN_DLD(fann_save,args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} fann_save(@var{ann}, @var{filename})\n\
Saves the FANN neural network @var{ann} in the file @var{filename}.\n\
@deftypefnx{Function File} @var{pos} = fann_save(@var{ann}, @var{filename}, \"fixed\")\n\
Saves the FANN neural network @var{ann} in the file @var{filename} as a fixed-point \
network.\n\
This is useful for training a network in floating points, and then later executing it \
in fixed point.\n\
The function returns the bit position @var{pos} of the fix point, which can be used to \
find out how accurate the fixed point network will be. A high value indicates high precision, \
and a low value indicates low precision.\n\
A negative value indicates very low precision, and a very strong possibility for overflow. \
(the actual fix point will be set to 0, since a negative fix point does not make sense).\n\
Generally, a fix point lower than 6 is bad, and should be avoided. The best way to avoid this, \
is to have less connections to each neuron, or just less neurons in each layer.\n\
The fixed point use of this network is only intended for use on machines that have no \
floating point processor, like an iPAQ. On normal computers the floating point version is \
actually faster.\n\
@strong{Note: it is currently not possible to load fixed-point networks into Octave.}\n\n\
@seealso{fann_create}\n\
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
    
    /* Check if the network should be saved as a fixed point network */
    if (args.length() == 3 && args(2).is_string()) {
        string str(args(2).string_value());
        lowercase(str);
        if (str == "fixed") {
            const int position = ann.save_fixed_fann(filename);
            return octave_value(position);
        } else {
            warning("Unknown string parameter: %s.", str.c_str());
        }
    }
    
    /* If it wasn't to be saved as a fixed point network */
    ann.save_fann(filename);
    return octave_value();
}

