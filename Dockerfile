# Use the official Python 3.10 image as the base
FROM python:3.10

# Install necessary packages for PHP
RUN apt-get update && apt-get install -y \
    php7.4 \
    php7.4-cli \
    php7.4-fpm \
    php7.4-json \
    php7.4-common \
    php7.4-mysql \
    php7.4-zip \
    php7.4-gd \
    php7.4-mbstring \
    php7.4-curl \
    php7.4-xml \
    php7.4-bcmath \
    php7.4-json

# Install Composer to manage PHP dependencies
RUN curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer

# Install necessary packages for Python
RUN pip3 install --upgrade pip

# Add additional instructions to install Python and PHP dependencies

# Set the working directory
WORKDIR /app

# Copy files into the container
COPY . /app

# Start PHP-FPM and other services (if needed)

# Run your application
CMD [ "python3", "app.py" ]






