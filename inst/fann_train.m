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
## @deftypefn {Function File} @var{mse} = fann_train(@var{ann}, @var{data}, @var{option1}, @var{value1}, ...)
## Trains the FANN neural network @var{ann} on @var{data} until a 
## desired error is reached, or until a maximum number of epochs is 
## surpassed. The reached mean squared error is returned.
## @var{data} can either be a data structure as the ones produces by
## fann_read_data or a filename.
## The functions accepts several optional parameters
## @table @code
## @item "MaxIterations"
## The maximum number of iterations. Defaults to 500000.
## @item "IterationsBetweenReports"
## The number of epochs between output gets printed on screen.
## The default is 1000.
## @item "DesiredError"
## The desired errer of the training. The default is 0.0001.
## @end table
##
## @strong{Example:}
## @example
## To train the network @var{ann} for one epoch on the data in the 
## file "xor.data":
## @code{@var{MSE} = fann_train(@var{ann}, "xor.data", "MaxIterations", 1);}
## @end example
##
## @seealso{fann_read_data, fann_test, and fann_shuffle}
## @end deftypefn

## Author: Søren Hauberg <hauberg at gmail dot com>
## 
## 2005-06-2 Søren Hauberg <hauberg at gmail dot com>
## * Initial revision

function fann_train(ann, arg, varargin)
  if (nargin < 2)
    error("At least two arguments must be given.\n");
  endif
  
  #ann = varargin{1};
  if (strcmp(class(ann), "fann") == 0)
    error("First argument must be a FANN\n");
  endif

  if (isstr(arg))
    # Do nothing
  elseif (isstruct(arg))
    filename = tmpnam();
    fann_save_train(arg, filename);
    arg = filename;
  else
    error("Second argument must either be a filename or a structure.\n");
  endif
 
  ## Call __fann_train_on_file
  cmd = "__fann_train_on_file(ann, arg";
  for i = 1:length(varargin)
    cmd = [cmd ", varargin{" num2str(i) "}"];
  endfor
  cmd = [cmd ");"];
  eval(cmd, "disp(lasterr)");
  
  if (exist("filename", "var"))
    delete(filename);
  endif
