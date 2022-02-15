console.log('loading core.js');

let jsroot = { version : "7.0.0" };

jsroot.require = function(arr) {
   let promises = [];
   arr.forEach(name => {
      let v = import('./'+name+'.js');
      promises.push(v);
   });

   return Promise.all(promises).then(res_arr => {
      for (let k = 0; k < res_arr.length; ++k)
         if (res_arr[k] && typeof res_arr[k] == 'object' && res_arr[k].default)
            res_arr[k] = res_arr[k].default;

      return Promise.all(res_arr);
   });
}

globalThis.JSROOT = jsroot;

export default jsroot;
