from flask import Flask, render_template, request, redirect, url_for, flash
import boto3
import os

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = 'uploads'

s3 = boto3.client('s3')

@app.route('/', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['file']
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        if file:
            filename = file.filename
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            s3.upload_file(os.path.join(app.config['UPLOAD_FOLDER'], filename), 'your_bucket_name', filename)
            flash('File successfully uploaded to S3')
            return redirect(url_for('upload_file'))
    return render_template('upload.html')

if __name__ == '__main__':
    app.run(debug=True, port=5000, host='0.0.0.0')
