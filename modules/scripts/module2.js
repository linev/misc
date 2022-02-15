console.log('load module2.js');

import jsroot from './core.js';

let module2 = jsroot.require(['module_base']).then(async () => {

   let p2 = await import("../src/painter2.js");

   return { name: "module2", Painter2: p2.Painter2 };
});

export { module2 as default };
