# ======================================================================
# DL6890 CNNs Exercise
#
#  Instructions
#  ------------
#
#  This file contains code that helps you get started on the CNN.
#  You will need to complete code in train_cnn.py
#
# ======================================================================

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import argparse
import gzip
import numpy
import pickle
import sys

import torch
from torchvision.datasets import MNIST
from train_cnn import ConvNet


# Set parameters for CNNs.
parser = argparse.ArgumentParser('CNN Exercise.')
parser.add_argument('--learning_rate',
                    type=float, default=0.1,
                    help='Initial learning rate.')
parser.add_argument('--num_epochs',
                    type=int,
                    default=30,
                    help='Number of epochs to run trainer.')
parser.add_argument('--beta',
                    type=float,
                    default=0.1,
                    help='decay rate of L2 regulization.')
parser.add_argument('--batch_size',
                    type=int, default=10,
                    help='Batch size. Must divide evenly into the dataset sizes.')
parser.add_argument('--input_data_dir',
                    type=str,
                    default='../data/mnist',
                    help='Directory to put the training data.')
parser.add_argument('--expanded_data',
                    type=str,
                    default='../data/mnist/mnist_expanded.pkl.gz',
                    help='Directory to put the extended mnist data.')
parser.add_argument('--log_dir',
                    type=str,
                    default='logs',
                    help='Directory to put logging.')
parser.add_argument('--visibleSize',
                    type=int,
                    default=str(28 * 28),
                    help='Used for gradient checking.')
parser.add_argument('--hiddenSize',
                    type=int,
                    default='100',
                    help='.')

FLAGS = None
FLAGS, unparsed = parser.parse_known_args()
mode = int(sys.argv[1])


# ======================================================================
#  STEP 0: Load data from the MNIST database
#
#  This loads our training and test data from the MNIST database files.
#  We have sorted the data for you in this so that you will not have to
#  change it.

# Load mnist data set

input_dir = FLAGS.input_data_dir
mnist_train = MNIST(root=input_dir, train=True, download=True)
train_set = {'trainX': mnist_train.train_data.type(torch.FloatTensor) / 255,
             'trainY': mnist_train.train_labels}

mnist_test = MNIST(root=input_dir, train=False, download=True)
test_set = {'testX': mnist_test.test_data.type(torch.FloatTensor) / 255,
            'testY': mnist_test.test_labels}


# ======================================================================
#  STEP 1: Train a baseline model.
#  This trains a feed forward neural network with one hidden layer.
#
#  Expected accuracy: 97.83%
#

if mode == 1:
  cnn = ConvNet(1)
  accuracy = cnn.train_and_evaluate(FLAGS, train_set, test_set)

  # Output accuracy
  print(20 * '*' + 'model 1' + 20 * '*')
  print('accuracy is %f' % (accuracy*100) + '%')
  print()


# ======================================================================
#  STEP 2: Use one convolutional layer.
#
#  Expected accuracy: 98.80%
#

if mode == 2:
  cnn = ConvNet(2)
  accuracy = cnn.train_and_evaluate(FLAGS, train_set, test_set)

  # Output accuracy.
  print(20 * '*' + 'model 2' + 20 * '*')
  print('accuracy is %f' % (accuracy*100) + '%')
  print()


# ======================================================================
#  STEP 3: Use two convolutional layers.
#
#  Expected accuracy: 99.02%
#

if mode == 3:
  cnn = ConvNet(3)
  accuracy = cnn.train_and_evaluate(FLAGS, train_set, test_set)

  # Output accuracy.
  print(20 * '*' + 'model 3' + 20 * '*')
  print('accuracy is %f' % (accuracy*100) + '%')
  print()


# ======================================================================
#  STEP 4: Replace sigmoid activation with ReLU.
#
#  Expected accuracy: 99.17%
#

if mode == 4:
  FLAGS.learning_rate = 0.03
  cnn = ConvNet(4)
  accuracy = cnn.train_and_evaluate(FLAGS, train_set, test_set)

  # Output accuracy.
  print(20 * '*' + 'model 4' + 20 * '*')
  print('accuracy is %f' % (accuracy*100) + '%')
  print()
