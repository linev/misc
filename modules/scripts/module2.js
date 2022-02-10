console.log('load module2.js');

export default JSROOT.require(['module_base.js']).then(res2 => {

   console.log('res2', res2[0]);

   return { name: "module2" };
});
