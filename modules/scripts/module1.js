console.log('load module1.js');

import jsroot from './core.js';

export default jsroot.require(['module_base']).then(async res1 => {

   let p1 = await import("../src/painter1.js");

   return { name: "module1", Painter1: p1.Painter1 };
});
