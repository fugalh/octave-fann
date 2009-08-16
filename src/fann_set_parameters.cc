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

DEFUN_DLD(fann_set_parameters, args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} {} fann_set_parameters(@var{ann}, @var{params})\n\
Sets the parameters of the the FANN neural network @var{ann}.\n\
The parameters are given in the struct @var{params} that may contain\n\
any of the following keys:\n\
\n\
@table @code\n\
@item TrainingAlgorithm\n\
Sets the training algorithm. The value of the key can be any of the\n\
following strings:\n\
@table @code\n\
@item incremental\n\
Standard backpropagation algorithm, where the weights are updated\n\
after each training pattern. This means that the weights are updated\n\
many times during a single epoch. For this reason some problems, will\n\
train very fast with this algorithm, while other more advanced\n\
problems will not train very well.\n\
@item batch\n\
Standard backpropagation algorithm, where the weights are updated\n\
after calculating the mean square error for the whole training set.\n\
This means that the weights are only updated once during a epoch. For\n\
this reason some problems, will train slower with this algorithm. But\n\
since the mean square error is calculated more correctly than in\n\
incremental training, some problems will reach a better solutions\n\
with this algorithm.\n\
@item rprop\n\
A more advanced batch training algorithm which achieves good results\n\
for many problems. The RPROP training algorithm is adaptive, and does\n\
therefore not use the learning_rate. Some other parameters can\n\
however be set to change the way the RPROP algorithm works, but it is\n\
only recommended for users with insight in how the RPROP training \n\
algorithm works.\n\
\n\
The actual learning algorithm used here is the iRPROP- training\n\
algorithm which is an variety of the standard RPROP training\n\
algorithm.\n\
@item quickprop\n\
A more advanced batch training algorithm which achieves good results\n\
for many problems. The quickprop training algorithm uses the\n\
LearningRate parameter along with other more advanced parameters, but\n\
it is only recommended to change these advanced parameters, for users\n\
with insight in how the quickprop training algorithm works.\n\
@end table\n\
@item LearningRate\n\
The value sets the learning rate of the training algorithm and must\n\
be a real scalar between 0 and 1.\n\
@item ActivationHidden\n\
Sets the activation function used by the neurons in the hidden\n\
layers. The value can be any of the following strings:\n\
@table @code\n\
@item Threshold\n\
@strong{Execution only} - Threshold activation function. This\n\
activation function gives output that is either 0 or 1.\n\
@item ThresholdSymmetric\n\
@strong{Execution only} - Threshold activation function. This\n\
activation function gives output that is either -1 or 1.\n\
@item Linear\n\
@strong{Can not be used in fixed point} - Linear activation function.\n\
This activation function gives output that is unbounded.\n\
@item Sigmoid\n\
Sigmoid activation function. One of the most used activation\n\
functions. This activation function gives output that is between 0\n\
and 1.\n\
@item SigmoidStepwise\n\
Stepwise linear approximation to sigmoid. Faster than sigmoid but a\n\
bit less precise. This activation function gives output that is\n\
between 0 and 1.\n\
@item SigmoidSymmetric\n\
Symmetric sigmoid activation function, AKA tanh. One of the most used\n\
activation functions. This activation function gives output that is\n\
between -1 and 1.\n\
@item SigmoidSymmetricStepwise\n\
Stepwise linear approximation to symmetric sigmoid. Faster than\n\
symmetric sigmoid but a bit less precise. This activation function\n\
gives output that is between -1 and 1.\n\
@end table\n\
@item ActivationOutput\n\
Sets the activation function used in the output layer. The value can\n\
be any of the strings described above in ActivationHidden.\n\
@item ActivationSteepnessHidden\n\
Set the steepness of the activation function of the hidden layers.\n\
The steepness defaults to 0.5 and a larger steepness will make the\n\
slope of the activation function more steep, while a smaller\n\
steepness will make the slope less steep. A large steepness is well\n\
suited for classification problems while a small steepness is well\n\
suited for function approximation.\n\
@item ActivationSteepnessOutput\n\
Set the steepness of the activation function of the output layer.\n\
See ActivationSteepnessHidden for details.\n\
@item TrainErrorFunction\n\
Sets the error functions used when calculating the error during\n\
training.\n\
@table @code\n\
@item Linear\n\
The basic linear error function which simply calculates the error as\n\
the difference between the real output and the desired output.\n\
@item TanH\n\
The TanH error function is an error function that makes large\n\
deviations stand out, by altering the error value used when training\n\
the network. The idea behind this is that it is worse to have 1\n\
output that misses the target by 100%, than having 10 outputs that\n\
misses the target by 10%.\n\
\n\
This is the default error function and it is usually better. It can\n\
however give poor results with high learning rates.\n\
@end table\n\
@item QuickPropDecay\n\
The decay is a small negative valued number which is the factor that\n\
the weights should become smaller in each iteration. This is used to\n\
make sure that the weights do not become too high during training.\n\
The default value for this parameter is -0.0001.\n\
@item QuickPropMu\n\
The mu factor is used to increase and decrease the step-size during\n\
quickprop training. The mu factor should always be above 1, since it\n\
would otherwise decrease the step-size when it was suppose to\n\
increase it. The default value for this parameter is 1.75.\n\
@item RPropIncreaseFactor\n\
The increase factor is a value larger than 1, which is used to\n\
increase the step-size during RPROP training. The default value for\n\
this parameter is 1.2.\n\
@item RPropDecreaseFactor\n\
The increase factor is a value smaller than 1, which is used to\n\
decrease the step-size during RPROP training. The default value for\n\
this parameter is 0.5.\n\
@item RPropDeltaMin\n\
The minimum step-size is a small positive number determining how\n\
small the minimum step may be. The default value for this parameter\n\
is 0.0.\n\
@item RPropDeltaMax\n\
The maximum step-size is a small positive number determining how\n\
small the minimum step may be. The default value for this parameter\n\
is 50.0.\n\
@end table\n\n\
@seealso{fann_get_parameters}\n\
@end deftypefn\n\
")
{
    if (args.length() < 2 || args(0).type_name() != "fann" || !args(1).is_map()) {
        error("First argument must be a fann and second must be a struct.\n");
        return octave_value();
    }
    
    // Get FANN
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& of = ((const oct_fann &)rep);

    // Get parameters map
    const Octave_map map = args(1).map_value();

    // For each parameter:
    typedef std::map<std::string, Cell>::const_iterator const_iterator;
    for (const_iterator iter = map.begin(); iter != map.end(); iter++) {
        string key(iter-> first);
        lowercase(key);
        const Cell value = iter->second;

        if (key == "trainingalgorithm") {
            if (!value(0,0).is_string()) {
                error("TrainingAlgorithm must be a string.\n");
                return octave_value();
            }
            string algo(value(0,0).string_value());
            lowercase(algo);
            if (algo == "incremental") {
                fann_set_training_algorithm(of.ann, FANN_TRAIN_INCREMENTAL);
            } else if (algo == "batch") {
                fann_set_training_algorithm(of.ann, FANN_TRAIN_BATCH);
            } else if (algo == "rprop") {
                fann_set_training_algorithm(of.ann, FANN_TRAIN_RPROP);
            } else if (algo == "quickprop") {
                fann_set_training_algorithm(of.ann, FANN_TRAIN_QUICKPROP);
            } else {
                warning("Unknown learning algorithm: %s", algo.c_str());
            }
                
        } else if (key == "learningrate") {
            if (!value(0,0).is_real_scalar()) {
                error("LearningRate must be a real scalar.\n");
                return octave_value();
            }
            float learning_rate = value(0,0).double_value();
            fann_set_learning_rate(of.ann, learning_rate);

        } else if (key == "activationhidden") {
            if (!value(0,0).is_string()) {
                error("ActivationHidden must be a string.\n");
                return octave_value();
            }
            string activation(value(0,0).string_value());
            lowercase(activation);
            if (activation == "threshold") {
                fann_set_activation_function_hidden(of.ann, FANN_THRESHOLD);
            } else if (activation == "thresholdsymmetric") {
                fann_set_activation_function_hidden(of.ann, FANN_THRESHOLD_SYMMETRIC);
            } else if (activation == "linear") {
                fann_set_activation_function_hidden(of.ann, FANN_LINEAR);
            } else if (activation == "sigmoid") {
                fann_set_activation_function_hidden(of.ann, FANN_SIGMOID);
            } else if (activation == "sigmoidstepwise") {
                fann_set_activation_function_hidden(of.ann, FANN_SIGMOID_STEPWISE);
            } else if (activation == "sigmoidsymmetric") {
                fann_set_activation_function_hidden(of.ann, FANN_SIGMOID_SYMMETRIC);
            } else if (activation == "sigmoidsymmetricstepwise") {
                fann_set_activation_function_hidden(of.ann, FANN_SIGMOID_SYMMETRIC_STEPWISE);
            } else {
                warning("Unknown Activation function: %s", activation.c_str());
            }
            
        } else if (key == "activationoutput") {
            if (!value(0,0).is_string()) {
                error("ActivationOutput must be a string.\n");
                return octave_value();
            }
            string activation(value(0,0).string_value());
            lowercase(activation);
            if (activation == "threshold") {
                fann_set_activation_function_output(of.ann, FANN_THRESHOLD);
            } else if (activation == "thresholdsymmetric") {
                fann_set_activation_function_output(of.ann, FANN_THRESHOLD_SYMMETRIC);
            } else if (activation == "linear") {
                fann_set_activation_function_output(of.ann, FANN_LINEAR);
            } else if (activation == "sigmoid") {
                fann_set_activation_function_output(of.ann, FANN_SIGMOID);
            } else if (activation == "sigmoidstepwise") {
                fann_set_activation_function_output(of.ann, FANN_SIGMOID_STEPWISE);
            } else if (activation == "sigmoidsymmetric") {
                fann_set_activation_function_output(of.ann, FANN_SIGMOID_SYMMETRIC);
            } else if (activation == "sigmoidsymmetricstepwise") {
                fann_set_activation_function_output(of.ann, FANN_SIGMOID_SYMMETRIC_STEPWISE);
            } else {
                warning("Unknown Activation function: ", activation.c_str());
            }
            
        } else if (key == "activationsteepnesshidden") {
            if (!value(0,0).is_real_scalar()) {
                error("ActivationSteepnessHidden must be a real scalar.\n");
                return octave_value();
            }
            fann_set_activation_steepness_hidden(of.ann, value(0,0).scalar_value());
            
        } else if (key == "activationsteepnessoutput") {
            if (!value(0,0).is_real_scalar()) {
                error("ActivationSteepnessOutput must be a real scalar.\n");
                return octave_value();
            }
            fann_set_activation_steepness_output(of.ann, value(0,0).scalar_value());
            
        } else if (key == "trainerrorfunction") {
            if (!value(0,0).is_string()) {
                error("TrainErrorFunction must be a string.\n");
                return octave_value();
            }
            string errorfun(value(0,0).string_value());
            lowercase(errorfun);
            if (errorfun == "linear") {
                fann_set_train_error_function(of.ann, FANN_ERRORFUNC_LINEAR);
            } else if (errorfun == "tanh") {
                fann_set_train_error_function(of.ann, FANN_ERRORFUNC_TANH);
            } else {
                warning("Unknown error function: %s",  errorfun.c_str());
            }
            
        } else if (key == "quickpropdecay") {
            if (!value(0,0).is_real_scalar()) {
                error("QuickpropDecay must be a real scalar.\n");
                return octave_value();
            }
            fann_set_quickprop_decay(of.ann, value(0,0).scalar_value());
            
        } else if (key == "quickpropmu") {
            if (!value(0,0).is_real_scalar()) {
                error("QuickpropMu must be a real scalar.\n");
                return octave_value();
            }
            fann_set_quickprop_mu(of.ann, value(0,0).scalar_value());

        } else if (key == "rpropincreasefactor") {
            if (!value(0,0).is_real_scalar()) {
                error("RPropIncreaseFactor must be a real scalar.\n");
                return octave_value();
            }
            fann_set_rprop_increase_factor(of.ann, value(0,0).scalar_value());

        } else if (key == "rpropdecreasefactor") {
            if (!value(0,0).is_real_scalar()) {
                error("RPropDecreaseFactor must be a real scalar.\n");
                return octave_value();
            }
            fann_set_rprop_decrease_factor(of.ann, value(0,0).scalar_value());

        } else if (key == "rpropdeltamin") {
            if (!value(0,0).is_real_scalar()) {
                error("RPropDeltaMin must be a real scalar.\n");
                return octave_value();
            }
            fann_set_rprop_delta_min(of.ann, value(0,0).scalar_value());

        } else if (key == "rpropdeltamax") {
            if (!value(0,0).is_real_scalar()) {
                error("RPropDeltaMax must be a real scalar.\n");
                return octave_value();
            }
            fann_set_rprop_delta_max(of.ann, value(0,0).scalar_value());

        } else {
            warning("Unknown parameter: %s", key.c_str());
        }
    }

    return octave_value();
}
