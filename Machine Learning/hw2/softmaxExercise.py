import argparse
import sys

import numpy as np
from numpy.random import randn, randint
from numpy.linalg import norm
import matplotlib.pyplot as plt
from scipy.sparse import coo_matrix

import utils

import torch
from torch.autograd import Variable

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
## STEP 1: Load data
#
#  In this section, we load the training instances and their labels.

if FLAGS.input_data == 'spiral':
  instances, labels, numClasses = utils.load_spiral_dataset()
elif FLAGS.input_data == 'flower':
  instances, labels, numClasses = utils.load_flower_dataset()
else:
  print('Wrong dataset specified. Select between "spiral" and "flower".')
  sys.exit(1)

inputSize = instances.shape[0]
numExamples = instances.shape[1]

dtype = torch.FloatTensor
ltype = torch.ByteTensor

##======================================================================
## STEP 2: Initialize parameters, set them up as Variables for autograd.
#

#  Instructions: Initialize W with a standard Gaussian * 0.01
#                Compute the groundTruth matrix, set as a variable.

# Randomly initialize parameters W and b.
groundTruth = coo_matrix((np.ones(numExamples, dtype = np.uint8),
                            (labels, np.arange(numExamples)))).toarray()
groundTruth = Variable(torch.Tensor(groundTruth))
W = torch.Tensor(numClasses, inputSize).normal_() * 0.01
W = Variable(W, requires_grad = True)
b = torch.Tensor(numClasses, 1).fill_(0)
b = Variable(b, requires_grad = True)

# Load training data into Variables that do not require gradients.
instances = Variable(torch.Tensor(instances))
labels = Variable(torch.Tensor(labels).type(ltype))

## ----------------------------------------------------------------

# Set hyper-parameters.
learning_rate = 1.0
decay = 0.001
num_epochs = 200

##======================================================================
## STEP 3: Gradient descent loop
#
# In this section, run gradient descent for num_epochs.
# At each epoch, first compute the loss variable, then update W and b
# using the gradient automatically computed by calling loss.backtrack().

for epoch in range(num_epochs):
  ## ---------- YOUR CODE HERE --------------------------------------
  #  Instructions: Compute the loss variable.

  loss = None

  Z = W.mm(instances) + b     # Z = W(k,T) X(n)
  Z = Z - Z.max(0)[0]         # Z = W(k,T)X(n) - max(W(k,T)X(n))
  exp_Z = torch.exp(Z)        # exp_Z = exp(W(k,T)X(n) - max(W(k,T)X(n))
  sum_exp = torch.sum(exp_Z, dim=0) # Σ exp(w(j,T)-max(W(j,T)X(n)))
  prob = exp_Z / sum_exp      # p(C(k)|X(n))
  log_prob = torch.log(prob)  #ln p(C(k)|X(n))

  # cost = -1/N * ΣΣ groundTruth*ln p(C(k)|X(n)) + decay/2 Σ W(k,T)W(k)
  loss = (-1.0 /numExamples)*torch.sum(groundTruth*log_prob)
  loss += (0.5 * decay) * torch.sum(W**2)

  ## ----------------------------------------------------------------

  if epoch % 10 == 0:
    print("Epoch %d: cost %f" % (epoch, loss))

  # Use autograd to compute the backward pass. This call will compute the
  # gradient of loss with respect to all Variables with requires_grad = True.
  # After this call W.grad and b.grad will be Variables holding the gradient
  # of the loss with respect to W and b respectively.
  loss.backward()

  # Update weights using gradient descent; W.data and b.data are Tensors,
  # W.grad and b.grad are Variables and W.grad.data and b.grad.data are
  # Tensors.
  W.data -= learning_rate * W.grad.data
  b.data -= learning_rate * b.grad.data

  # Manually zero the gradients after updating weights
  W.grad.data.zero_()
  b.grad.data.zero_()


##======================================================================
## STEP 4: Test on training data
#

## ---------- YOUR CODE HERE --------------------------------------
#  Instructions: The array pred should contain the predictions of the
#  softmax model on hte training examples.

pred = None

pred = (W.mm(instances) + b).max(0)[1]
print((W.mm(instances) + b).max(0)[1])
pred = pred.data.numpy()


## ----------------------------------------------------------------

acc = np.mean(labels.data.numpy() == pred)
print('Accuracy: %0.3f%%.' % (acc * 100))

# Accuracy is the proportion of correctly classified images
# After 200 epochs, the results for our implementation were:
#
# Spiral Accuracy: 53.3%
# Flower Accuracy: 47.0%


##======================================================================
## STEP 5: Plot the decision boundary
#
utils.plot_decision_boundary(lambda x: np.argmax((W.data.numpy().dot(x) + b.data.numpy()),
                                                 axis = 0),
                             instances.data.numpy(), labels.data.numpy())
plt.title("Softmax Regression")
plt.savefig(FLAGS.input_data + '-boundary.png')
plt.show()
