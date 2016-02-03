#! /usr/bin/env node
;
(function () {
    "use strict";

    var _ = require('lodash');
    var walk = require('walk');
    var fs = require('fs');
    var path = require('path');
    var exec = require('child_process').exec;

    var fn_fileHandler, fn_errorsHandler, fn_endHandler;

    fn_errorsHandler = function errorsHandler_(root, nodeStatsArray, next) {
        nodeStatsArray.forEach(function (n) {
            console.error("[ERROR] " + n.name)
            console.error(n.error.message || (n.error.code + ": " + n.error.path));
        });
        next();
    };

    fn_endHandler = function endHandler_() {
        console.log("all done");
    };

    var walker = walk.walk("out", {followLinks: false});

    fn_fileHandler = function fileHandler_(root, fileStat_, next) {
        if (_.startsWith(fileStat_.name, 'test_')) {
            var p = path.resolve(root, fileStat_.name);
            exec(p, function (error_, stdout_, stderr_) {
                if(stdout_)console.log(stdout_);
                if(stderr_)console.log(stderr_);
                if (error_ !== null) {
                    console.error("[ERROR] test failed: " + p)
                    process.exit(1);
                }else
                {
                    console.log("[INFO] test ok:  " + p)
                }
            });
        }
        next();
    };

    walker.on("file", fn_fileHandler);
    walker.on("errors", fn_errorsHandler); // plural
    walker.on("end", fn_endHandler);


}());