console.log('load module1.js');

import jsroot from './core.js';

let module1 = jsroot.require(['module_base']).then(async () => {
   let p1 = await import("../src/painter1.js");

   return { name: "module1", Painter1: p1.Painter1 };
});

export { module1 as default };
