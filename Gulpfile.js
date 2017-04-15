const gulp = require('gulp');
const pug = require('gulp-pug');
const flatmap = require('gulp-flatmap');
const rename = require('gulp-rename');
const uglify = require('gulp-uglify');
const cssimport = require('gulp-cssimport');
const del = require('del');
const fs = require('fs');
const path = require('path');

gulp.task('clean', function(){
  return del(['dist', 'build'])
});

gulp.task('markdown', function() {
    return gulp.src('md/*')
        .pipe(flatmap(function(stream, file){
            const contents = file.contents.toString('utf8');
            return gulp.src("template.pug")
                .pipe(pug({
                    pretty: true,
                    data: {
                        contents: contents
                    }
                }))
                .pipe(rename({
                    basename: path.basename(file.path, '.md')
                }))
        }))
        .pipe(gulp.dest('dist/'))
});


gulp.task('scripts', function() {
    return gulp.src('scripts/*')
        .pipe(uglify())
        .pipe(gulp.dest('build/scripts'))
});

gulp.task('graphics', function() {
    return gulp.src('graphics/*')
        .pipe(gulp.dest('dist/graphics'))
});

gulp.task('css', function() {
    return gulp.src('css/*')
        .pipe(cssimport())
        .pipe(gulp.dest('build/css'))
});

gulp.task('default',
    gulp.series(
    'clean',
    gulp.parallel('css', 'scripts', 'graphics'),
    'markdown'
  )
);
