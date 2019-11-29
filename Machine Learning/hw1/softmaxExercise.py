## Softmax Exercise

#  Instructions
#  ------------
#
#  This file contains code that helps you get started on the
#  softmax exercise. You will need to write the softmax cost function and
#  the softmax prediction function in softmax.py. You will also need to write
#  code in computeNumericalGradient.py.
#  For this exercise, you will not need to change any code in this file,
#  or any other files other than the ones mentioned above.

import argparse
import sys

import numpy as np
from numpy.random import randn, randint
from numpy.linalg import norm
import matplotlib.pyplot as plt

from softmax import softmaxCost, softmaxPredict
from computeNumericalGradient import computeNumericalGradient

import utils

parser = argparse.ArgumentParser('Softmax Exercise.')
parser.add_argument('-i', '--input_data',
                    type=str,
                    default='spiral',
                    help='Dataset: select between "spiral" and "flower".')
parser.add_argument('-d', '--debug',
                    action='store_true',
                    help='Used for gradient checking.')

FLAGS, unparsed = parser.parse_known_args()

##======================================================================
## STEP 1: Loading data
#
#  In this section, we load the training examples and their labels.

if FLAGS.input_data == 'spiral':
  instances, labels, numClasses = utils.load_spiral_dataset()
elif FLAGS.input_data == 'flower':
  instances, labels, numClasses = utils.load_flower_dataset()
else:
  print('Wrong dataset specified. Select between "spiral" and "flower".')
  sys.exit(1)

inputSize = instances.shape[0]
numExamples = instances.shape[1]

# For debugging purposes, you may wish to reduce the size of the input data
# in order to speed up gradient checking.
# Here, we create synthetic dataset using random data for testing

if FLAGS.debug:
  inputSize = 8
  instances, labels = randn(8, 100), randint(0, numClasses, 100, dtype = np.uint8)

# Randomly initialize parameters
W = 0.01 * randn(numClasses, inputSize)
b = np.zeros((numClasses, 1))

##======================================================================
## STEP 2: Gradient checking
#
#  As with any learning algorithm, you should always check that your
#  gradients are correct before learning the parameters.
#

if FLAGS.debug:
  decay = 0.001
  cost, dW, db = softmaxCost(W, b, numClasses, inputSize, decay, instances, labels)
  W_numGrad = computeNumericalGradient(lambda x: softmaxCost(x, b, numClasses, inputSize,
                                                             decay, instances, labels), W)

  # Use this to visually compare the gradients side by side.
  print(np.stack((W_numGrad.ravel(), dW.ravel())).T)

  # Compare numerically computed gradients with those computed analytically.
  diff = norm(W_numGrad - dW) / norm(W_numGrad + dW)
  print(diff)
  sys.exit(0)
  # The difference should be small.
  # In our implementation, these values are usually less than 1e-7.


##======================================================================
## STEP 3: Learning parameters
#
#  Once you have verified that your gradients are correct, you can start
#  training your softmax regression code using gradient descent.

# Set hyper-parameters.
learning_rate = 1.0
decay = 0.001
num_epochs = 200

# Gradient descent loop.
for epoch in range(num_epochs):
  # Implement softmaxCost in softmax.py, to compute cost and gradients.
  cost, dW, db = softmaxCost(W, b, numClasses, inputSize, decay, instances, labels)
  if epoch % 10 == 0:
    print("Epoch %d: cost %f" % (epoch, cost))

  # Update parameters.
  W += -learning_rate * dW
  b += -learning_rate * db

##======================================================================
## STEP 4: Testing on training data
#
#  You should now test your model against the training examples.
#  To do this, you will first need to write softmaxPredict
#  (in softmax.py), which should return predictions
#  given a softmax model and the input data.

pred = softmaxPredict(W, b, instances)

acc = np.mean(labels == pred)
print('Accuracy: %0.3f%%.' % (acc * 100))

# Accuracy is the proportion of correctly classified images
# After 200 epochs, the results for our implementation were:
#
# Spiral Accuracy: 53.3%
# Flower Accuracy: 47.0%

##======================================================================
## STEP 5: Plot the decision boundary.
#
utils.plot_decision_boundary(lambda x: softmaxPredict(W, b, x), instances, labels)
plt.title("Softmax Regression")
plt.savefig(FLAGS.input_data + '-boundary.png')
plt.show()
