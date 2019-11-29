import numpy as np

def computeNumericalGradient(J, theta):
  """ Compute numgrad = computeNumericalGradient(J, theta)

  theta: a matrix of parameters
  J: a function that outputs a real-number and the gradient.
  Calling y = J(theta)[0] will return the function value at theta.
  """

  # Initialize numgrad with zeros
  numgrad = np.zeros(theta.shape)


  ## ---------- YOUR CODE HERE --------------------------------------
  # Instructions:
  # Implement numerical gradient checking, and return the result in numgrad.
  # You should write code so that numgrad[i][j] is (the numerical approximation to) the
  # partial derivative of J with respect to theta[i][j], evaluated at theta.
  # I.e., numgrad[i][j] should be the (approximately) partial derivative of J with
  # respect to theta[i][j].
  #
  # Hint: You will probably want to compute the elements of numgrad one at a time.

  # define ε = 0.0001
  epislon = 0.0001
  for i in range(theta.shape[0]):
    for j in range(theta.shape[1]):

  # Run J(θ−ε) function to find the cost_min
        theta_min = theta.copy()
        theta_min[i,j] = theta_min[i,j] - epislon
        cost_min, dW, db = J(theta_min)

  # Run J(θ+ε) function to find the cost_max
        theta_max = theta.copy()
        theta_max[i,j] = theta_max[i,j] + epislon
        cost_max, dW, db = J(theta_max)

  # numerical approximation of derivative
  # G(num(θ))=(J(θ+ε)−J(θ−ε)) / 2ε
        numgrad[i][j] = (cost_max - cost_min) / (2*epislon)

  ## ---------------------------------------------------------------
  return numgrad
