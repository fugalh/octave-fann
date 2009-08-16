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
## @deftypefn {Function File} @var{data} = fann_shuffle_data(@var{data})
## Changes the order of the elements in @var{data}.
## @var{data} must be a structure as those created by fann_read_data.
##
## @seealso{fann_read_data, and fann_merge_data}
## @end deftypefn

## Author: Søren Hauberg <hauberg at gmail dot com>
## 
## 2005-06-2 Søren Hauberg <hauberg at gmail dot com>
## * Initial revision

function data = fann_shuffle_data(data)
  
  try 
    input  = data.input;
    target = data.target;
  catch
    error("DATA must be a struct containing 'input' and 'target'.\n");
  end_try_catch

  if (size(input,1) != size(target,1))
    error("input and target must have same number of data.\n");
  endif

  ## Create shuffle index
  n = size(input,1);
  idx = 1:n;
  J = 1 + round(rand(1,n) * (n-1));
  for i = 1:n
    # swap i and j
    tmp = idx(i);
    idx(i) = idx(J(i));
    idx(J(i)) = tmp;
  endfor

  ## Shuffle the data
  data.input = input(idx, :);
  data.target= target(idx,:);
