#! /usr/bin/env node

;
(function() {

    var _ = require('lodash');
    var spawn = require('child_process').spawn;
    var chokidar = require('chokidar');

    var fn_cmd, fn_watch, debounce_delay;

    debounce_delay = 250;

    fn_cmd = function(evt_, path_) {
        var spawned_cmd = spawn('make', ['tests']);
        console.log(['[INFO] ', evt_, ' => ', path_].join(''));

        spawned_cmd.stdout.on('data', function(data_) {
            process.stdout.write(data_);
        });
        spawned_cmd.stderr.on('data', function(data_) {
            process.stderr.write(data_);
        });
        spawned_cmd.on('close', function(code_) {
            process.stdout.write('[INFO] build exit = ' + code_ + '\n');
        });
    };

    fn_watch = function() {
        var watcher = chokidar.watch('src', {
            ignored: /[\/\\]\./
        });
        watcher.on('all', _.debounce(fn_cmd, debounce_delay));
    };

    fn_watch();

}());