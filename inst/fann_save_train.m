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
## @deftypefn {Function File} fann_save_train(@var{data}, @var{filename})
## Saves @var{data} into the file @var{filename}.
## @var{data} must be a structure as the created by fann_read_data.
##
## @seealso{fann_read_data}
## @end deftypefn

## Author: Søren Hauberg <hauberg at gmail dot com>
## 
## 2005-06-2 Søren Hauberg <hauberg at gmail dot com>
## * Initial revision

function fann_save_train(data, filename)
  if (nargin != 2)
    usage("fann_save_train(DATA, FILENAME)");
  endif
  
  try
    input  = data.input;
    target = data.target;
  catch
    error("Data must be a struct containing both input and target.\n");
  end_try_catch

  if (size(input,1) != size(target,1))
    error("input and target must have the same number of data elements.\n");
  endif

  fid = fopen(filename, "w");
  if (fid == -1)
    error("Bad filename\n");
  endif

  ## Write the data
  fprintf(fid, "%d %d %d\n", size(input,1), size(input,2), size(target,2));
  format = [repmat("%.30f ", 1, size(input, 2)) "\n" ...
            repmat("%.30f ", 1, size(target,2)) "\n"];
  fprintf(fid, format, [input target]');
  fclose(fid);

