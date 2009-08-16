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
## @deftypefn {Function File} @var{mse} = fann_test(@var{ann}, @var{data})
## Calculates the mean square error for a set of data.
## @var{data} must be a structue as the created by fann_read_data.
## @deftypefnx{Function File} @var{mse} = fann_test(@var{ann}, @var{filename})
## As above except the data is given as a filename.
## @deftypefnx{Function File} @var{mse} = fann_test(@var{ann}, @var{input}, @var{target})
## As above except the input and target is given as seperate arguments.
## Both @var{input} and @var{target} must be real vectors, and their 
## sizes must match those of the networks input and output layers.
##
## @seealso{fann_read_data, fann_train, and fann_create}
## @end deftypefn

## Author: Søren Hauberg <hauberg at gmail dot com>
## 
## 2005-06-2 Søren Hauberg <hauberg at gmail dot com>
## * Initial revision

function MSE = fann_test(ann, arg1, arg2)
  if (nargin < 2 && nargin > 3)
    error("Two or three arguments must be given.\n");
  endif

  if (strcmp(class(ann), "fann") == 0)
    error("First argument must be a FANN.\n");
  endif

  if (nargin == 2)
    if (isstr(arg1))
      MSE = __fann_test_data_from_file(ann, arg1);
    elseif (isstruct(arg1))
      filename = tmpnam();
      fann_save_train(arg1, filename);
      MSE = __fann_test_data_from_file(ann, filename);
      delete(filename);
    else
      error("Second argument must either be a filename or a structure.\n");
    endif
  
  else # nargin == 3
    if (isrealmatrix(arg1) && isrealmatrix(arg2))
      data.input  = arg1(:)';
      data.target = arg2(:)';
      filename = tmpnam();
      fann_save_train(data, filename);
      MSE = __fann_test_data_from_file(ann, filename);
      delete(filename);
    else
      error("Second and third arguments must be real matrices.\n");
    endif
  endif
endfunction

function b = isrealmatrix(M)
  b = ismatrix(M) && isreal(M);
endfunction
