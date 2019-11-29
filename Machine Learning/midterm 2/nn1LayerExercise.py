import argparse
import sys

import numpy as np
import matplotlib.pyplot as plt

import utils

import torch
from torch.autograd import Variable

parser = argparse.ArgumentParser('NN with 1 Hidden Layer Exercise.')
parser.add_argument('-i', '--input_data',
                    type=str,
                    default='spiral',
                    help='Dataset: select between "spiral" and "flower".')
parser.add_argument('-d', '--debug',
                    action='store_true',
                    help='Used for gradient checking.')

FLAGS, unparsed = parser.parse_known_args()

np.random.seed(1)

##======================================================================
## STEP 1: Load data
#
#  In this section, we load the training instances and their labels.

if FLAGS.input_data == 'spiral':
  X, y, n_y = utils.load_spiral_dataset()
  # Set hyper-parameters.
  n_h = 100
  decay = 0.001
  learning_rate = 1
  num_epochs = 10000
elif FLAGS.input_data == 'flower':
  X, y, n_y = utils.load_flower_dataset()
  # Set hyper-parameters.
  n_h = 20
  decay = 0
  learning_rate = 0.05
  num_epochs = 20000
else:
  print('Wrong dataset specified. Select between "spiral" and "flower".')
  sys.exit(1)

n_x = X.shape[0]

dtype = torch.FloatTensor
ltype = torch.LongTensor


##======================================================================
## STEP 2: Initialize parameters, set up Variables for autograd.
#

# Load data into Variables that do not require gradients.
X = Variable(torch.from_numpy(X.T).type(dtype), requires_grad = False)
y = Variable(torch.from_numpy(y).type(ltype), requires_grad = False)


## -------------------- YOUR CODE HERE ------------------------------
# Define model to be a NN with one hidden ReLU layer and linear outputs.
# Define loss_fn to compute the cross entropy loss.
# Define the optimizer to run SGD with learning_rate and the weight decay.

# define model, where the n_x is the input size, n_h is the hidden dimension
# n_y is the output size
model = torch.nn.Sequential(
      torch.nn.Linear(n_x, n_h),
      torch.nn.ReLU(),
      torch.nn.Linear(n_h, n_y),
)

# define the loss function which will compute the cross entropy loss
loss_fn = torch.nn.CrossEntropyLoss()

# deifne optimizer that will update the weights of the model.
# it will update model.parameters() with lr = learning_rate, weight_decay = decay
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate, weight_decay=decay)

## ----------------------------------------------------------------


##======================================================================
## STEP 3: Gradient descent loop
#
for epoch in range(num_epochs):
## ---------- YOUR CODE HERE --------------------------------------
# In this section, run gradient descent for num_epochs.
# At each epoch:
#   - first compute the model predictions;
#   - then compute the loss between predictions and true labels;
#          print the loss every 100 epochs;
#   - then zero de gradients through the optimizer object;
#   - then run backpropagation on the loss.

   # 1. compute the model predictions
   y_pred = model(X)
   # 2. compute the loss between predictions and the true labels
   loss = loss_fn(y_pred, y)

   if epoch % 100 == 0:
       print("Epoch %d: loss %f" % (epoch, loss))
   # 3. zero de gradients through the optimizer object
   optimizer.zero_grad()
   # 4. run backpropagation on the loss
   loss.backward()
   # 5. call the step function to make an update to its parameters
   optimizer.step()



## ----------------------------------------------------------------


##======================================================================
## STEP 4: Test on training data
#
#  Now test your model against the training examples.
#  The array pred should contain the predictions of the softmax model.

pred = model(X).max(1)[1]# YOUR CODE HERE

pred = pred.data.numpy()
print(pred)

acc = np.mean(y.data.numpy() == pred)
print('Accuracy: %0.3f%%.' % (acc * 100))

# Accuracy is the proportion of correctly classified images
# After 200 epochs, the results for our implementation were:
#
# Spiral Accuracy: 99.00%
# Flower Accuracy: 87.00%

utils.plot_decision_boundary(lambda x: model(x), X, y)
plt.title("Neural Network")
plt.savefig(FLAGS.input_data + '-boundary.png')
plt.show()
