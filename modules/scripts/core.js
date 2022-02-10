(function( factory ) {
   if (typeof exports === 'object' && typeof module !== 'undefined') {
      // processing with Node.js

      //  mark JSROOT as used with Node.js
      exports.batch_mode = exports.nodejs = (typeof global==='object') && global.process && (Object.prototype.toString.call(global.process) === '[object process]');

      factory(exports);

      global.JSROOT = exports;
   } else {

      let jsroot = {};

      factory(jsroot);

      globalThis.JSROOT = jsroot;

   }
} (function(JSROOT) {

   JSROOT.version = "7.0.0";

   JSROOT.require = function(arr) {
      let promises = [];
      arr.forEach(name => {
         promises.push(import('./'+name));
      });

      return Promise.all(promises).then(res_arr => {
         for (let k = 0; k < res_arr.length; ++k)
            if (res_arr[k] && typeof res_arr[k] == 'object' && res_arr[k].default)
               res_arr[k] = res_arr[k].default;

         return Promise.all(res_arr);
      });
   }

}));
