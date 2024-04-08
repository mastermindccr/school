import keras
import numpy as np
from sklearn.metrics import accuracy_score
model = keras.models.load_model('109550060_tensorFlow.h5') # load saved model for testing
x_test = np.load("x_test.npy").astype('float32')/255
y_test = np.load("y_test.npy")

y_pred = model.predict(x_test)
y_pred = np.argmax(y_pred, axis=1)

print("Accuracy of my model on test set: ", accuracy_score(y_test, y_pred)) # examine the test result
input("Press Enter to continue...")