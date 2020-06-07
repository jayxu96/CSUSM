'''
This script shows how to predict stock prices using a basic RNN
'''
import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np

from google.cloud import storage
import flask
import io
import os


# train Parameters
seq_length = 7
data_dim = 5
output_dim = 1
learning_rate = 0.01
iterations = 500

app = flask.Flask(__name__)

def MinMaxScaler(data):
    ''' Min Max Normalization
    Parameters
    ----------
    data : numpy.ndarray
        input data to be normalized
        shape: [Batch size, dimension]
    Returns
    ----------
    data : numpy.ndarry
        normalized data
        shape: [Batch size, dimension]
    References
    ----------
    .. [1] http://sebastianraschka.com/Articles/2014_about_feature_scaling.html

    '''
    numerator = data - np.min(data, 0)
    denominator = np.max(data, 0) - np.min(data, 0)
    # noise term prevents the zero division
    return numerator / (denominator + 1e-7)

# build datasets
def build_dataset(time_series, seq_length):
    dataX = []
    dataY = []
    for i in range(0, len(time_series) - seq_length):
        x = time_series[i:i + seq_length, :]
        y = time_series[i + seq_length, [-1]]  # Next close price
        print(x, "->", y)
        dataX.append(x)
        dataY.append(y)
    return np.array(dataX), np.array(dataY)

def training(fileName):
    xy = np.loadtxt(fileName, delimiter=',')
    xy = xy[::-1]  # reverse order (chronically ordered)

    # train/test split
    train_size = int(len(xy) * 1.0)
    train_set = xy[0:train_size]
    # Scale each
    train_set = MinMaxScaler(train_set)

    trainX, trainY = build_dataset(train_set, seq_length)

    print(trainX.shape)  # (505, 7, 5)
    print(trainY.shape)


    model = tf.keras.Sequential();
    model.add(tf.keras.layers.LSTM(units=1, input_shape=(seq_length, data_dim)))
    model.add(tf.keras.layers.Dense(units=output_dim, activation='tanh'))
    model.summary()

    model.compile(loss='mean_squared_error', optimizer=tf.keras.optimizers.Adam(lr=learning_rate))
    model.fit(trainX, trainY, epochs=iterations)
    tf.saved_model.save(model, "saved_model/my_model")

    return model


def main():
    fileName = 'google-daily.csv'
    training(fileName)


if __name__ == "__main__":
    main()
