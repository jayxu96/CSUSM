import numpy
import sys
import time
import torch
import torch.nn as NN
import torch.optim as optim
import torch.nn.functional as F
from torch.autograd import Variable


# Change the shape of the input tensor
class ViewOP(torch.nn.Module):
  def __init__(self, *shape):
    super(ViewOP, self).__init__()
    self.shape = shape

  def forward(self, input):
    return input.view(self.shape)


########### Convolutional neural network class ############
class ConvNet(object):
  def __init__(self, mode):
    self.mode = mode
    if torch.cuda.is_available():
        self.use_gpu = True
    else:
        self.use_gpu = False

  # Baseline model.
  def model_1(self, X, hidden_size, class_num):
    # ======================================================================
    # One fully connected layer.
    #
    # ----------------- YOUR CODE HERE ----------------------
    #
    # Use nn.sequential as the constructor to build the model
    model = NN.Sequential()
    # Define the first linear transformation to the incoming data
    # with only fully connected layer, the input will be the size of X
    # which is 28 * 28, output will be the 100 neurons
    model.add_module("linear1", NN.Linear(X.shape[1], hidden_size))
    # Add a sigmoid activation
    model.add_module("sigmoid", NN.Sigmoid())
    # Define the output layer, input is the 100 sigmoid neurons,
    # output is the 10 neuron ouput layer with softmax
    model.add_module("linear2", NN.Linear(hidden_size, class_num))
    model.add_module("softmax", NN.LogSoftmax())

    return model

  # Add one convolutional layer.
  def model_2(self, X, hidden_size, class_num):
    # ======================================================================
    # One convolutional layer + one fully connected layer.
    #
    # ----------------- YOUR CODE HERE ----------------------
    #
    # Use nn.sequential as the constructor to build the model
    model = NN.Sequential()
    # Add a convolutional layer, the input_channels=1,output_channels=20
    # kernel_size=5,stride=1, the output layer will be 20 * 24 * 24
    # (28-5+1)/1 = 24 where 28 is the original input Width, 5 is the kernel_size
    # second 1 is the stride
    model.add_module("conv1", NN.Conv2d(1,20,5))
    # Add a 2*2 max pooling, the output layer will be 20 * 12 * 12
    model.add_module("pool1", NN.MaxPool2d(2, 2))
    # Transform the pooling layer
    model.add_module("reshape", ViewOP(-1, 20 * 12 * 12))
    # Fully connected layer from step1 where change the input size to the pooling
    # layer size
    model.add_module("linear1", NN.Linear(20 * 12 * 12, hidden_size))
    model.add_module("sigmoid", NN.Sigmoid())
    model.add_module("linear2", NN.Linear(hidden_size, class_num))
    model.add_module("softmax", NN.LogSoftmax())


    return model

  # Use two convolutional layers.
  def model_3(self, X, hidden_size, class_num):
    # ======================================================================
    # Two convolutional layers + one fully connnected layer.
    #
    # ----------------- YOUR CODE HERE ----------------------
    #
    # Use nn.sequential as the constructor to build the model
    model = NN.Sequential()
    # Copy the first convolutional-pooling layer
    model.add_module("conv1", NN.Conv2d(1,20,5))
    model.add_module("pool1", NN.MaxPool2d(2,2))
    # Add a second convolutional layer, the input_channels=20,output_channels=40
    # kernel_size=5,stride=1, the output layer will be 40 * 8 * 8
    # (12-5+1)/1 = 8 where 12 is the pooling layer 1 input Width,
    # 5 is the kernel_size, second 1 is the stride
    model.add_module("conv2", NN.Conv2d(20,40,5))
    # Add a 2*2 max pooling, the output layer will be 40 * 4 * 4
    model.add_module("pool2", NN.MaxPool2d(2,2))
    # Transform the pooling layer
    model.add_module("reshape", ViewOP(-1, 40 * 4 * 4))
    # Fully connected layer from step1 where change the input size to the pooling
    # layer 2 size
    model.add_module("linear1", NN.Linear(40 * 4 * 4, hidden_size))
    model.add_module("sigmoid", NN.Sigmoid())
    model.add_module("linear2", NN.Linear(hidden_size, class_num))
    model.add_module("softmax", NN.LogSoftmax())


    return model

  # Replace sigmoid with ReLU.
  def model_4(self, X, hidden_size, class_num):
    # ======================================================================
    # Two convolutional layers + one fully connected layer, with ReLU.
    #
    # ----------------- YOUR CODE HERE ----------------------
    #
    model = NN.Sequential()
    model.add_module("conv1", NN.Conv2d(1,20,5))
    model.add_module("pool1", NN.MaxPool2d(2,2))
    model.add_module("conv2", NN.Conv2d(20,40,5))
    model.add_module("pool2", NN.MaxPool2d(2,2))
    model.add_module("reshape", ViewOP(-1, 40 * 4 * 4))
    model.add_module("linear1", NN.Linear(40 * 4 * 4, hidden_size))
    # Replace sigmoid with ReLU
    model.add_module("ReLU", NN.ReLU())
    model.add_module("linear2", NN.Linear(hidden_size, class_num))
    model.add_module("softmax", NN.LogSoftmax())

    return model

  # Evaluate the trained model on test set.
  def evaluate_model(self, model, X, Y):
    pred_Y = model(X)
    _, idx = torch.max(pred_Y, dim=1)

    # Move tensor from GPU to CPU.
    if self.use_gpu:
      idx = idx.cpu()
      Y = Y.cpu()

    idx = idx.data.numpy()
    Y = Y.data.numpy()
    accuracy = numpy.mean(idx == Y)

    return accuracy

  # Entry point for training and evaluation.
  def train_and_evaluate(self, FLAGS, train_set, test_set):
    class_num     = 10
    num_epochs    = FLAGS.num_epochs
    batch_size    = FLAGS.batch_size
    learning_rate = FLAGS.learning_rate
    hidden_size   = FLAGS.hiddenSize
    beta          = FLAGS.beta

    # Input data
    trainX, trainY = train_set['trainX'], train_set['trainY']
    testX, testY = test_set['testX'], test_set['testY']

    train_size = trainX.size()[0]
    test_size = testX.size()[0]

    # Use CPU or GPU. True: use GPU; False: use CPU.
    self.use_gpu = False

    # Set random number generator seed.
    torch.manual_seed(1024)
    if self.use_gpu:
      torch.cuda.manual_seed_all(1024)
    numpy.random.seed(1024)

    # Model 1: baseline.
    if self.mode == 1:
      trainX = trainX.view(train_size, -1)
      testX = testX.view(test_size, -1)

    # Models 2, 3, 4, 5, 6, 7.
    elif self.mode > 1 and self.mode < 8:
      trainX = trainX.view(train_size, 1, 28, 28)
      testX = testX.view(test_size, 1, 28, 28)

    # Model number is undefined.
    else:
      print("The input model number is undefined!")
      sys.exit()

    model = eval("self.model_" + str(self.mode))(
              trainX, hidden_size, class_num
            )

    if self.use_gpu:
      ftype = torch.cuda.FloatTensor # float type
      itype = torch.cuda.LongTensor # int type
      model.cuda()
    else:
      ftype = torch.FloatTensor # float type
      itype = torch.LongTensor # int type

    # Create optimizer.
    optimizer = optim.SGD(model.parameters(), lr=learning_rate)

    # Train the model.
    for i in range(num_epochs):
      print(21 * '*', 'epoch', i+1, 21 * '*')
      start_time = time.time()

      # Put model in training mode.
      model.train()

      s = 0
      while s < train_size:
        e = min(s + batch_size, train_size)
        batch_x = trainX[s : e]
        batch_y = trainY[s : e]

        X = Variable(batch_x.type(ftype), requires_grad=False)
        Y = Variable(batch_y.type(itype), requires_grad=False)

        optimizer.zero_grad()
        loss_func = F.nll_loss(model(X), Y)
        loss_func.backward()
        optimizer.step()
        s = e

      end_time = time.time()
      print ('the training took: %d(s)' % (end_time - start_time))

      # Put model in evaluation mode.
      model.eval()

      # Create variables for test set.
      X = Variable(testX.type(ftype), requires_grad=False)
      Y = Variable(testY.type(itype), requires_grad=False)

      print("Accuracy of the trained model on test set is", self.evaluate_model(model, X, Y))

    return self.evaluate_model(model, X, Y)
