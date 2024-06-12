# Image Recognition Project

This project focuses on building a deep learning model for image recognition using convolutional neural networks (CNNs). The goal is to classify images into different categories based on their content.

## Overview

The project involves:

- **Data Preparation**: Organizing image data into appropriate directories and loading them into a DataFrame for processing.
- **Model Development**: Constructing a CNN architecture using Keras with TensorFlow backend.
- **Training**: Training the model on the prepared image dataset.
- **Evaluation**: Evaluating the model's performance on a separate test dataset.
- **Deployment**: Deploying the trained model for inference on new images.

## Installation

To run the project locally, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/your_username/image-recognition-project.git
   ```

2. Install the required dependencies:

   ```bash
   pip install -r requirements.txt
   ```

## Usage

1. Organize your image data into separate directories for training and testing.

2. Modify the `config.py` file to specify the paths to your training and testing data.

3. Run the preprocessing script to prepare the data:

   ```bash
   python preprocess.py
   ```

4. Train the model using the following command:

   ```bash
   python train.py
   ```

5. Evaluate the model's performance:

   ```bash
   python evaluate.py
   ```

6. Use the trained model for inference on new images:

   ```bash
   python predict.py --image path/to/your/image.jpg
   ```

