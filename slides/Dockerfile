FROM node:7.9.0
RUN npm install gulp-cli -g
WORKDIR /opt/mccwa/
COPY package.json /opt/mccwa/
RUN npm install
COPY scripts /opt/mccwa/scripts
COPY graphics /opt/mccwa/graphics
COPY css /opt/mccwa/css
COPY template.pug /opt/mccwa/
COPY md /opt/mccwa/md
COPY Gulpfile.js /opt/mccwa/
RUN gulp