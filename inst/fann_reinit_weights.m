## Copyright (C) 2005 Søren Hauberg
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

## -*- texinfo -*-
## @deftypefn {Function File} fann_reinit_weights(@var{ann})
## @deftypefnx {Function File} fann_reinit_weights(@var{ann}, "random")
## Reinitializes the weights of of the FANN neural network @var{ann}
## to random numbers between -1 and 1.
## @deftypefnx {Function File} fann_reinit_weights(@var{ann}, "random", [@var{min} @var{max}])
## Reinitializes the weights of of the FANN neural network @var{ann}
## to random numbers between @var{min} and @var{max}.
## @deftypefnx {Function File} fann_reinit_weights(@var{ann}, "nguyen&widrow", @var{data})
## @deftypefnx {Function File} fann_reinit_weights(@var{ann}, "nguyen", @var{data})
## @deftypefnx {Function File} fann_reinit_weights(@var{ann}, "widrow", @var{data})
## Reinitializes the weights of @var{ann} using the algorithm developed
## by Derrick Nguyen and Bernard Widrow to set the weights in such a
## way as to speed up training. This technique is not always successful,
## and in some cases can be less efficient than a purely random 
## initialization.
## @var{data} must be the data set to be used for training, either 
## given as a filename or as a structure like the one created by
## fann_read_data.
##
## @seealso{fann_create}
## @end deftypefn

## Author: Søren Hauberg <hauberg at gmail dot com>
## 
## 2005-06-2 Søren Hauberg <hauberg at gmail dot com>
## * Initial revision

function fann_reinit_weights(ann, method, arg)
  ## Input checking
  if (nargin < 1 || strcmp(class(ann), "fann") == 0)
    error("First argument must be a FANN neural network.\n");
  endif

  if (nargin < 2)
    method = "random";
  endif

  if (!isstr(method))
    error("Second argument must be a string.\n");
  endif

  ## Which method should we use
  switch tolower(method)
    case "random"
      if (nargin < 3)
        fann_randomize_weights(ann);
      else
        fann_randomize_weights(ann, arg);
      endif
      
    case {"nguyen", "widrow", "nguyen&widrow"}
      if (nargin < 3)
        error(["A third argument containing the data must always be supplied " ...
               "when using Nguyen & Widrow.\n"]);
      endif

      if (isstr(arg))
        __fann_init_weights_on_file(ann, arg);
      elseif (isstruct(arg))
        filename = tmpnam();
        fann_save_train(arg, filename);
        __fann_init_weights_on_file(ann, filename);
        delete(filename);
      else
        error("Data argument must either be a filename or a struct.\n");
      endif
    otherwise
      error("Unknown method: %s.\n", method);
  endswitch
  
