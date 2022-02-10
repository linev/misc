console.log('load module1.js');

export default JSROOT.require(['module_base.js']).then(res1 => {

  console.log('res1', res1[0]);

   return { name: "module1" };
});
