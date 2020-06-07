import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np
from google.cloud import storage
import flask
import os

from stock_gcp_v2 import MinMaxScaler, build_dataset


app = flask.Flask(__name__)

seq_length = 7
data_dim = 5
output_dim = 1
learning_rate = 0.01
iterations = 500

def getCloudobject(certentialName,bucketName,objectName,fileName):
   client = storage.Client.from_service_account_json(certentialName)
   bucket = client.get_bucket(bucketName)
   blob = bucket.get_blob(objectName)
   blob.download_to_filename(fileName)
   print("download successful!")

def uploadObject(certentialName,bucketName,objectName,fileName):
    client = storage.Client.from_service_account_json(certentialName)
    bucket = client.get_bucket(bucketName)
    blob = bucket.blob(objectName)
    blob.upload_from_filename(filename=fileName)
    url = "http://storage.cloud.google.com/" + bucketName + objectName
    return url

def load_model(fileName):
    new_model = tf.keras.models.load_model(fileName)
    new_model.summary()
    return new_model

def load_data(dataName):
    xy = np.loadtxt(dataName, delimiter=',')
    xy = xy[::-1]  # reverse order (chronically ordered)
    # train/test split
    train_size = int(len(xy) * 0.90)
    test_set = xy[train_size - seq_length:]  # Index from [train_size - seq_length] to utilize past sequence

    # Scale each
    #train_set = MinMaxScaler(train_set)
    test_set = MinMaxScaler(test_set)

    #trainX, trainY = build_dataset(train_set, seq_length)
    testX, testY = build_dataset(test_set, seq_length)

    return testX,testY

def test(model,testX,testY,outputName):
    test_predict = model.predict(testX)
    print("--------------------PRINT TEST PREDCIT-------------------- ")
    #print(test_predict)
    # Plot predictions
    fig, ax = plt.subplots()
    fig.canvas.draw()
    labels = ['0','2','4','6','8','10','12','14','16','18']
    lablesy = ['0','500','600','700','800','900']

    x1 = [0,15,30,45,60,75,90,105,120,135,150]
    plt.xticks(x1, labels)
    y1 = [0,0.2,0.4,0.6,0.8,1.0]
    plt.yticks(y1,lablesy)
    #ax.set_xticklabels(labels, rotation='vertical')
    #ax.set_yticklabels(lablesy)
    plt.plot(testY, label = 'Actual price')
    plt.plot(test_predict, label = 'Predicted price')
    plt.title("Alphabet Inc.")
    plt.xlabel("Week")
    plt.ylabel("Stock Price ($)")
    plt.legend()
    if not os.path.isdir("static"):
        os.mkdir("static")

    plt.savefig(outputName)
    plt.show()

@app.route("/")
def home():
    return flask.render_template("index.html")

@app.route("/predict", methods=["POST"])
def predict():
    params = flask.request.json
    if(params == None):
        params = flask.request.args

    if(params != None):
        fileName =flask.request.form.get('fileName')
        bucket = "jay-ml"
        certentialName = 'jayml.json'
        objectName = 'stock/' + fileName
        getCloudobject(certentialName,bucket,objectName,fileName)
        path = 'saved_model/my_model'
        model = load_model(path)
        dataX,dataY = load_data(fileName)
        image_name = "static/test.png"
        test(model,dataX,dataY,image_name)
        upload_name = "stock/test.png"
        uploadObject(certentialName,bucket,upload_name,image_name)
        return flask.render_template("predict.html",image = image_name)

if __name__ == "__main__":
     #predict()
     app.run(host='0.0.0.0', port= 6001)
