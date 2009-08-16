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
## @deftypefn {Function File} @var{merged_data} = fann_merge_data(@var{data1}, @var{data2})
## Merges the two FANN data sets @var{data1} and @var{data2} into
## @var{merged_data}.
##
## @seealso{fann_read_data}
## @end deftypefn

## Author: Søren Hauberg <hauberg at gmail dot com>
## 
## 2005-06-2 Søren Hauberg <hauberg at gmail dot com>
## * Initial revision

function out = fann_merge_data(data1, data2)
  if (nargin != 2)
    usage("fann_save_train(DATA1, DATA");
  endif
  
  try
    input1  = data1.input;
    target1 = data1.target;
    input2  = data2.input;
    target2 = data2.target;
  catch
    error("DATA1 and DATA2 must be structs containing both input and target.\n");
  end_try_catch

  if (size(input1,1) != size(target1,1) || size(input2,1) != size(target2,1))
    error("input and target must have the same number of data elements.\n");
  endif

  if (size(input1,2) != size(input2,2) || size(target1,2) != size(target2,2))
    error("Data sizes doesn't match.\n");
  endif

  out.input = [input1;  input2];
  out.target= [target1; target2];

endfunction
