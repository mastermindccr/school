#!/bin/sh

extension=`echo "$1" | awk -F"." '{printf $(NF)'}`
echo extension: "$extension" >> /home/mastermindccr/HW3/test.log
if [ "$extension" = "exe" ]; then
	logger -r user.notice "$1 violate file detected. Uploaded by $UPLOAD_USER"
	echo "log1 complete!" >> /home/mastermindccr/HW3/test.log
	logger -r user.notice "$1 violate file detected. Uploaded by $UPLOAD_USER"
	echo "log2 complete!" >> /home/mastermindccr/HW3/test.log
	mv "$1" "/home/ftp/hidden/.exe/"
	echo "$1 is moved!" >> /home/mastermindccr/HW3/test.log
fi

echo "$1 uploaded by ${UPLOAD_USER}!" >> /home/mastermindccr/HW3/test.log
