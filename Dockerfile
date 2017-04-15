FROM node:7.9.0
RUN npm install gulp-cli -g
WORKDIR /opt/mccwa/
COPY slides/package.json /opt/mccwa/
RUN npm install
COPY slides/scripts /opt/mccwa/scripts
COPY slides/graphics /opt/mccwa/graphics
COPY slides/css /opt/mccwa/css
COPY slides/template.pug /opt/mccwa/
COPY slides/md /opt/mccwa/md
COPY slides/Gulpfile.js /opt/mccwa/
RUN gulp