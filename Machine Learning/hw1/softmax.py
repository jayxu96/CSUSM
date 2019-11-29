import numpy as np
from scipy.sparse import coo_matrix

def softmaxCost(W, b, numClasses, inputSize, decay, data, labels):
  """Computes and returns the (cost, gradient)

  W, b - the weight matrix and bias vector parameters
  numClasses - the number of classes
  inputSize - the size D of each input vector
  decay - weight decay parameter
  data - the D x N input matrix, where each column data[:,n] corresponds to
         a single sample
  labels - an N x 1 matrix containing the labels corresponding for the input data
  """

  N = data.shape[1]

  groundTruth = coo_matrix((np.ones(N, dtype = np.uint8),
                            (labels, np.arange(N)))).toarray()
  cost = 0;
  dW = np.zeros((numClasses, inputSize))
  db = np.zeros((numClasses, 1))

  ## ---------- YOUR CODE HERE --------------------------------------
  #  Instructions: Compute the cost and gradient for softmax regression.
  #                You need to compute dW, dW, and cost.
  #                The groundTruth matrix might come in handy.

  Z = W.dot(data) + b   # Z = W(k,T) X(n)
  Z = Z - np.max(Z, axis=0, keepdims=True) # Z = W(k,T)X(n) - max(W(k,T)X(n))
  exp_Z = np.exp(Z)  # exp_Z = exp(W(k,T)X(n) - max(W(k,T)X(n)))
  sum_exp = np.sum(exp_Z, axis=0,keepdims=True) # Σ exp(w(j,T)-max(W(j,T)X(n)))
  prob = exp_Z / sum_exp # p(C(k)|X(n))
  log_prob = np.log(prob) #ln p(C(k)|X(n))
  # cost = -1/N * ΣΣ groundTruth*ln p(C(k)|X(n))
  cost = (-1.0 / N) * np.sum(np.multiply(groundTruth, log_prob))
  # cost = -1/N * ΣΣ groundTruth*ln p(C(k)|X(n)) + decay/2 Σ W(k,T)W(k)
  cost += (0.5 * decay) * np.sum(W**2)

  # Gradient of the loss with respect to weights
  # dW = -1/N * (G[k,n] - p(C(k)|X(n)))X(n,T) + decay*W(k,T)
  dW = (-1.0 / N) * (groundTruth - prob).dot(data.T)
  dW += decay * W

  # Gradient of the loss with respect to biases
  # db = -1/N * Σ (G[k,n] - p(C(k)|X(n)))
  db = (-1.0 / N) * np.sum((groundTruth-prob),axis=1, keepdims=True)
  return cost, dW, db


def softmaxPredict(W, b, data):
  """Computes and returns the softmax predictions in the input data.

  W, b - model parameters trained using softmaxTrain,
         a numClasses x D matrix and a numClasses x 1 column vector.
  data - the D x N input matrix, where each column data[:,n] corresponds to
         a single sample.
  """

  #  Your code should produce the prediction matrix pred,
  #  where pred(i) is argmax_c P(c | x(i)).

  ## ---------- YOUR CODE HERE ------------------------------------------
  #  Instructions: Compute pred using W and b, assuming that the labels
  #                start from 0.

  # C = argmax W(k,T)X
  Z = W.dot(data) + b
  pred = np.argmax(Z, axis=0)
  print(pred)

  # ---------------------------------------------------------------------

  return pred
