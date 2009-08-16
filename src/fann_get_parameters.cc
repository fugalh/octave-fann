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

DEFUN_DLD(fann_get_parameters, args, ,
"-*- texinfo -*-\n\
@deftypefn {Function File} @var{params} = fann_get_parameters(@var{ann})\n\
Gets the parameters of the the FANN neural network @var{ann}.\n\
The parameters are given in the struct @var{params} that contains\n\
the following keys:\n\
\n\
@table @code\n\
@item TrainingAlgorithm\n\
The training algorithm. The value of the key can be any of the\n\
following strings:\n\
@table @code\n\
@item Incremental\n\
Standard backpropagation algorithm, where the weights are updated\n\
after each training pattern. This means that the weights are updated\n\
many times during a single epoch. For this reason some problems, will\n\
train very fast with this algorithm, while other more advanced\n\
problems will not train very well.\n\
@item Batch\n\
Standard backpropagation algorithm, where the weights are updated\n\
after calculating the mean square error for the whole training set.\n\
This means that the weights are only updated once during a epoch. For\n\
this reason some problems, will train slower with this algorithm. But\n\
since the mean square error is calculated more correctly than in\n\
incremental training, some problems will reach a better solutions\n\
with this algorithm.\n\
@item RProp\n\
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
@item Quickprop\n\
A more advanced batch training algorithm which achieves good results\n\
for many problems. The quickprop training algorithm uses the\n\
LearningRate parameter along with other more advanced parameters, but\n\
it is only recommended to change these advanced parameters, for users\n\
with insight in how the quickprop training algorithm works.\n\
@end table\n\
@item LearningRate\n\
The value of the learning rate of the training algorithm and is\n\
a real scalar between 0 and 1.\n\
@item ActivationHidden\n\
The activation function used by the neurons in the hidden\n\
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
The activation function used in the output layer. The value can\n\
be any of the strings described above in ActivationHidden.\n\
@item ActivationSteepnessHidden\n\
The steepness of the activation function of the hidden layers.\n\
@item ActivationSteepnessOutput\n\
The steepness of the activation function of the output layer.\n\
@item TrainErrorFunction\n\
The error functions used when calculating the error during\n\
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
increase it.\n\
@item RPropIncreaseFactor\n\
The increase factor is a value larger than 1, which is used to\n\
increase the step-size during RPROP training.\n\
@item RPropDecreaseFactor\n\
The increase factor is a value smaller than 1, which is used to\n\
decrease the step-size during RPROP training.\n\
@item RPropDeltaMin\n\
The minimum step-size is a small positive number determining how\n\
small the minimum step may be.\n\
@item RPropDeltaMax\n\
The maximum step-size is a small positive number determining how\n\
small the minimum step may be.\n\
@end table\n\n\
@seealso{fann_set_parameters}\n\
@end deftypefn\n\
")
{
    if (args.length() != 1 || args(0).type_name() != "fann") {
        error("First argument must be a FANN.\n");
        return octave_value();
    }
    
    // Get FANN
    const octave_base_value& rep = args(0).get_rep();
    const oct_fann& of = ((const oct_fann &)rep);

    // Create parameters map
    Octave_map map;

    // TrainingAlgorithm
    const unsigned int training_algorithm = fann_get_training_algorithm(of.ann);
    string str_training_algorithm;
    
    if (training_algorithm == (unsigned int)FANN_TRAIN_INCREMENTAL) {
        str_training_algorithm = "Incremental";
    } else if (training_algorithm == (unsigned int)FANN_TRAIN_BATCH) {
        str_training_algorithm = "Batch";
    } else if (training_algorithm ==(unsigned int) FANN_TRAIN_RPROP) {
        str_training_algorithm = "RProp";
    } else if (training_algorithm == (unsigned int)FANN_TRAIN_QUICKPROP) {
        str_training_algorithm = "Quickprop";
    } else {
        error("Internal Error in the FANN binding:\n");
        error("Unknown training algorithm in get_parameters\n");
        return octave_value();
    }
    map.assign("TrainingAlgorithm", str_training_algorithm);
    
    // LearningRate
    const double learning_rate = fann_get_learning_rate(of.ann);
    map.assign("LearningRate", learning_rate);
    
    // ActivationHidden
    struct fann_layer *hidden_layer = of.ann->first_layer+1;
    struct fann_layer *output_layer = of.ann->last_layer-1;

    const unsigned int activation_hidden = hidden_layer->first_neuron->activation_function;
    string str_activation_hidden;
    
    if (activation_hidden == (unsigned int)FANN_THRESHOLD) {
        str_activation_hidden = "Threshold";
    } else if (activation_hidden == (unsigned int)FANN_THRESHOLD_SYMMETRIC) {
        str_activation_hidden = "ThresholdSymmetric";
    } else if (activation_hidden ==(unsigned int)FANN_LINEAR) {
        str_activation_hidden = "Linear";
    } else if (activation_hidden == (unsigned int)FANN_SIGMOID) {
        str_activation_hidden = "Sigmoid";
    } else if (activation_hidden == (unsigned int)FANN_SIGMOID_STEPWISE) {
        str_activation_hidden = "SigmoidStepwise";
    } else if (activation_hidden == (unsigned int)FANN_SIGMOID_SYMMETRIC) {
        str_activation_hidden = "SigmoidSymmetric";
    } else if (activation_hidden == (unsigned int)FANN_SIGMOID_SYMMETRIC_STEPWISE) {
        str_activation_hidden = "SigmoidSymmetricStepwise";
    } else {
        error("Internal Error in the FANN binding:\n");
        error("Unknown activation_hidden in get_parameters\n");
        return octave_value();
    }
    map.assign("ActivationHidden", str_activation_hidden);

    // ActivationOutput
    const unsigned int activation_output = output_layer->first_neuron->activation_function;
    string str_activation_output;
    
    if (activation_output == (unsigned int)FANN_THRESHOLD) {
        str_activation_output = "Threshold";
    } else if (activation_output == (unsigned int)FANN_THRESHOLD_SYMMETRIC) {
        str_activation_output = "ThresholdSymmetric";
    } else if (activation_output ==(unsigned int)FANN_LINEAR) {
        str_activation_output = "Linear";
    } else if (activation_output == (unsigned int)FANN_SIGMOID) {
        str_activation_output = "Sigmoid";
    } else if (activation_output == (unsigned int)FANN_SIGMOID_STEPWISE) {
        str_activation_output = "SigmoidStepwise";
    } else if (activation_output == (unsigned int)FANN_SIGMOID_SYMMETRIC) {
        str_activation_output = "SigmoidSymmetric";
    } else if (activation_output == (unsigned int)FANN_SIGMOID_SYMMETRIC_STEPWISE) {
        str_activation_output = "SigmoidSymmetricStepwise";
    } else {
        error("Internal Error in the FANN binding:\n");
        error("Unknown activation_output in get_parameters\n");
        return octave_value();
    }
    map.assign("ActivationOutput", str_activation_output);

    // ActivationSteepnessHidden
    const double activation_steepness_hidden = hidden_layer->first_neuron->activation_steepness;
    map.assign("ActivationSteepnessHidden", activation_steepness_hidden);
    
    // ActivationSteepnessOutput
    const double activation_steepness_output = output_layer->first_neuron->activation_steepness;
    map.assign("ActivationSteepnessOutput", activation_steepness_output);
    
    // TrainErrorFunction
    const unsigned int train_error_function = fann_get_train_error_function(of.ann);
    string str_train_error_function;
    
    if (train_error_function == (unsigned int)FANN_ERRORFUNC_LINEAR) {
        str_train_error_function = "Linear";
    } else if (train_error_function == (unsigned int)FANN_ERRORFUNC_TANH) {
        str_train_error_function = "TanH";
    } else {
        error("Internal Error in the FANN binding:\n");
        error("Unknown train_error_function in get_parameters\n");
        return octave_value();
    }
    map.assign("TrainErrorFunction", str_train_error_function);
    
    // QuickpropDecay
    const double quickprop_decay = fann_get_quickprop_decay(of.ann);
    map.assign("QuickpropDecay", quickprop_decay);
    
    // QuickpropMu
    const double quickprop_mu = fann_get_quickprop_mu(of.ann);
    map.assign("QuickpropMu", quickprop_mu);
    
    // RPropIncreaseFactor
    const double RPropIncreaseFactor = fann_get_rprop_increase_factor(of.ann);
    map.assign("RPropIncreaseFactor", RPropIncreaseFactor);
    
    // RPropDecreaseFactor
    const double RPropDecreaseFactor = fann_get_rprop_decrease_factor(of.ann);
    map.assign("RPropDecreaseFactor", RPropDecreaseFactor);
    
    // RPropDeltaMin
    const double RPropDeltaMin = fann_get_rprop_delta_min(of.ann);
    map.assign("RPropDeltaMin", RPropDeltaMin);
    
    // RPropDeltaMax
    const double RPropDeltaMax = fann_get_rprop_delta_max(of.ann);
    map.assign("RPropDeltaMax", RPropDeltaMax);
    
    // Return the parameters
    return octave_value(map);
}
