'''
This script shows how to predict stock prices using a basic RNN
'''
import matplotlib.pyplot as plt
import tensorflow as tf
import numpy as np
from google.cloud import storage
import flask
from flask_restful import reqparse, Api, Resource
import io
import os


from stock_gcp_v2 import MinMaxScaler, build_dataset


# train Parameters
seq_length = 7


app = flask.Flask(__name__)
api = Api(app)

# argument parsing
parser = reqparse.RequestParser()
parser.add_argument('query', type=str)


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

class PredictStockPrice(Resource):
    def get(self):
        # use parser and find the user's query
        args = parser.parse_args()
        user_query = args['query']
        fileName = user_query
        path = 'saved_model/my_model'
        bucket = "jay-ml"
        certentialName = 'jayml.json'
        objectName = 'stock/' + fileName
        print(objectName)
        getCloudobject(certentialName,bucket,objectName,fileName)
        model = load_model(path)
        dataX,dataY = load_data(fileName)
        image_name = 'static/test.png'
        test(model,dataX,dataY,image_name)
        upload_name = "stock/test.png"
        uploadObject(certentialName,bucket,upload_name,image_name)
        #https://storage.cloud.google.com/jay-ml/stock/test.png
        url = 'https://storage.cloud.google.com/'+ bucket + objectName
        output = {'Prediction chart name': url}

        return output


api.add_resource(PredictStockPrice,'/')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port= 5000)

# Open, High, Low, Volume, Close
#fileName = input("Enter the dataset: ")


#image_data = open(image_name, "rb")
#s3.Bucket(bucket).put_object(Key=image_name, Body=img_data,
#                                ContentType="image/png", ACL="public-read")


#print(url)
